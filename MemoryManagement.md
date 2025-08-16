## MECC Addtional Details:
- MECC is designed to alleviate cognitive load from developers by eliminating cyclic references from occurring via root-aware objects with reference-counting. The algorithm's main purpose is to eliminate any tracing, marking, and pausing (stop-the-world) that typical garbage collectors suffer from, which is done by having meta-data consisting of two equal bit-length fields (architecture dependent). 
  - One field is specifically used for tracking the root-reference address in a reference chain to determine ownership and prevent cyclic references from objects sharing a common root-reference from affecting the second field. The second field is the reference-count field. In the case of 64-bit systems, the two fields are 8-bytes each.
  -  As mentioned, one field is a pointer that stores the memory address of the local or global root-object that serves as the root of the entire reference-tree and is used for determining if an object's ownership can be transferred to a new root-owner - which is a conditional operation dependent on if the object maintains a reference count greater than zero.
  - The reference-count field solely increments when a different object of a different root-address (object B) references to an object with another root-address (object A). This behavior is significant as you are guaranteed no cyclic references occur on reference-count updates, therefore reference-counts can reliably be used for determining when an object should be deallocated.
  - Further clarification - only objects that enclose a value (ie structs) may opt-in to MECC by incorporating the MECC header into a struct.
  This specifically allows the choice of system programming with manual memory management when desired and the option to opt-in to managed memory per the developers' discretion. Furthermore - enclosing objects maintain the sum of reference counts of their individual fields. This allows the entire object to have predictable allocation and deallocation triggers.
  - The benefits MECC has over conventional tracing garbage collection algorithms is the ability to prevent cyclic cycles of references from forming whenever reference assignments are used in the language via compiler-inserted instructions.
  This solves the main-flaw of reference counting which is susceptible to cyclic references causing memory leaks in programs where circular references may occur. The assignment of a root-object is transversal and determinable in constant time, and incrementing a reference-counter can be achieved atomically or concurrently through managed synchronization for multithreaded programs.
  - Due to MECC's simplistic design, it's possible to perform memory-management without any pauses at runtime since no tracing is required, nor any marking actions - simply sweeping on reference counts hitting zero or program termination.
  This also allows the option to improve the language ergonomics to allow developers to focus less on the memory-management and more-so on the programs they are writing - which leads to the first design decision of Clyth to be pass-by-reference by default.
  This removes the need for pointer syntax to reference or dereference objects and their fields entirely, bringing Clyth closer to feeling like a higher-level language akin to Java and JS, TS, and Python. Additionally, pass-by-value is deliberately opt-in, meaning developers must implement logic to clone objects if they so need copies of objects instead of direct manipulation of them across function calls - however - primitive literal types such as integers, floats, doubles, and chars are all pass-by-value - as they're not objects.

## Additional Memory Management options explored:
- Scope-based Inheritance Borrow Checker Reference Table Scheme (SIBCRT)
  - Each scope will maintain its own SIBCRT which tracks strong and weak references amongst objects to prevent cyclic references from occurring in mark-and-sweep-like garbage-collected languages, theoretically reducing garbage collection overhead if done in a minimal fashion.
  - Strong references can be made by any object to another, as long as the object storing the reference doesn't already have the object it's referring to stored as a strong reference in one of its parents. IE objA objB and objC exist, objA stores a strong reference to objC, objC stores a strong reference to objB, but objB can't store a strong reference to objA nor objC if it needs to, therefore objB will resort to weak-references to both objA and objC to prevent a cycle of references from occurring.
  - SIBCRT lookups will be performed whenever a reference is assigned by an object to another on invocation of the "=" operator. The SIBCRT for that scope will be referenced and the parents of the object being pointed to will be checked for existing strong references before assigning the type of reference to use.
  - Owners of an object can be multiple in number, they will simply be appended to a queue of owners that will "inherit" the ownership of the object that all the other owners are currently referencing as well. In the case an owner goes out of scope or is no longer referring to an object with multiple owners, then only that owner is removed from the queue of owners. Once all owners no longer maintain the reference to the shared object, that object will be garbage-collected as no strong references will remain after the last owner loses the reference to the shared object.
  - When exiting a scope, the SIBCRT will:
      - upgrade all weak-references of an object that is being returned to strong-references before returning from the scope.
      - transfer the "owner" of the object being returned to the parent-scope's SIBCRT
  - Async Atomic Tracing Garbage Collector Component
      - Garbage Collector will run atomically and asynchronously to the main program, preventing GC overhead induced from common stop-the-world collectors at runtime.
      - Will only be invoked whenever a strong-reference goes out of scope or an owner of an object from the queue of owners is freed or no longer maintains the strong reference it had on its referenced object.

- Tracing garbage collector
  - Exists in many modern garbage-collected languages like Go, Java, C#, JS, and Python
  - Measurable overhead on CPU and Memory for marking and sweeping
  - Stop-the-world or incremental collection not suitable for systems-level programming

- EC Architecture (Entanglement Component(s))
  - EC Architecture consists of EC entities and EC sockets. Inspired by exploration of Websockets and Smart-Pointers. The entire purpose of this architecture is to have a lean garbage collector that mimics manual memory management but is automated via a borrow-checker-like 'entanglement' system. Entanglement can be best described as a subscription of components (known as EC entities) and sockets (known as EC sockets) that manage them. This allows for a runtime-oriented implementation that allows for greater flexibility than a typical reference-counting or ahead of time (AOT) borrow-checker implementation, all while stripping the usual Garbage-Collection 'helper' cycles to determine cyclic references,
      - EC Entities:
      - Wrappers around data or objects that 
      - EC Sockets:
      - Maintain references to EC entities where ownership of entities is managed through transfer operations.
      - Difference between smart-pointers in C++ and EC sockets is this: EC sockets manage transfer operations for reference passing, copying of pointers or objects, being thread-safe (atomic), and NOT relying on reference counting while allowing for a flexible borrow-checker-like Garbage collection scheme at runtime.
      - EC sockets manage memory/garbage collection of the EC entities they have ownership over. As the name implies, each EC socket is allowed to manage an EC entity if the EC socket has ownership over that particular This action of entities being managed by sockets is called 'entanglement', which is what makes the 'Entanglement-Component' acronym in the EC Architecture. As a result of a single-owner, sockets have method that can be invoked to perform various null-safe or non-null-safe operations for transferring reference-ownership, non-ownership-references, copy of entity that's currently entangled. This greatly simplifies memory management to where GC tracing for cyclic cycles is no longer a concern and memory deallocations can occur asynchronously in a 'chain-like' effect, similar to how manual memory-management in C and C++ languages is conducted but more-inline with C++ destructors being automatically called.
      - Transfer operations:
      - transfer: Transfer of ownership over to another EC socket, performing null-safety-checks to ensure the EC entity object itself isn't null.
      - ftransfer: Fast-transfer of ownership over to another EC Socket, not performing any null-safety-checks.
      - ctransfer: Copy-transfer, which copies the original object's memory and transfers ownership of that copied entity to the new EC socket.
      - Detail Specification:
      - This requires a EC-centric standard library implementation that covers all primitives of the language to allow the EC system to have first-class support. An additional standard library implementation for non-EC primitives will be made for the sake of advanced systems programming for those who require it, especially in embedded system environments/platforms. These standard libraries will contain mostly primitives and datastructures that are the building-blocks to creating seamless objects and constructs with for developers. In the case of the EC-standard library, its main focus is to provide EC-friendly datastructures, primitives, and types, allowing developers to not concern themselves with managing the memory, instead focusing on the implementation details of their programs with the expectation of automated memory management.
          - Standard library variants would allow each library variant a unique modularity that allows any type of GC to be supported. The intended design is for each standard library type to be namespaced under the ```stdlib.{variant name}``` nomenclature for separation of concerns.
          - Cross-compatibility between EC and non-EC components will be managed through a 'wrapper' class that any non-EC component can be wrapped into to allow cross-compatibility.
          - Datastructures for the EC-centric standard library will primarily consist of EC sockets, and optimized with integration for EC entities.
          - Developers can specify if the specific datastructure or EC socket being assigned an EC entity is receiving direct ownership or a reference. All references are considered weak-references unless an ownership transfer is used for that particular reference EC entity that contains that pointer value.
          - Nested EC sockets are possible, which will allow for multi-dimensional data-structures like 3D+ arrays/lists.
- Simplified variation of Option 3 by orchestrating smart-pointers
  - Option 3's EC Architecture can be mapped to a much simpler smart-pointer reference - where at any-point in time the smart-pointer holds onto it's owned data as a strong-reference, and only when transferring ownership does it transition to a weak-pointer (in behavior) where it no longer affects the deallocation of the previously owned data.
  - Implementation can be as straightforward as a struct with a generic pointer to the target type/storage type, a deallocation function, and a flag/bit-toggle to alter the deallocation behavior for the object on strong to weak reference transition - and vice-versa as well.
  ```java
      {
      SampleClass myClass = new SampleClass();
      SampleClass myClass2 = myClass; // ownership transfer (if in the same scope as the original object's creation).
      SampleClass myClass3ref = &myClass2; // This is strictly a reference to myClass2, not an ownership transfer;
      SampleClass myClass3 = myClass2; // Ownership transfer - myClass2 is now a reference and myClass3 the owner.

      // At this point, myClass, myClass2, and myClass3ref are all references to the first instance of SampleClass.
      // Though, unlike Rust, all the variables that're now references can still operate on the same instance and will not throw an error.
      // You may still operate on myClass3 as the owner moving forwards.
      } // At this point, if myClass3 hasn't been returned out of this scope, it's responsible for cleaning up the SampleClass instance.
  ```
  - Function calls are NOT able to pass ownership along as Rust does. The only exception to this is if you pass a container object which stores the instance/object - ie an array/list.