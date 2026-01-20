### Thread-Safe Singleton

**Your Task**
Implement a class `Singleton` that ensures only one instance exists throughout the program lifecycle.

**Requirements:**
1. **Thread Safety:** Initialization must be thread-safe.
2. **Efficiency:** Do not use expensive locks if C++ static initialization rules suffice (Meyers Singleton).
3. **No Copies:** Delete copy constructor and assignment operator.
4. `get()` method should return a reference to the instance.
