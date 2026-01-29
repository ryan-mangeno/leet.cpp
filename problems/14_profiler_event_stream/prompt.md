### Generate Event Stream from Profiler Samples

**Your Task**
A sampling profiler collects chronological snapshots of call stacks. Convert these samples into a stream of Begin/End events.

**How it Works:**
- Each `Sample` has an elapsed time (ms) and a call stack (outermost → innermost)
- Generate `Begin` events when functions enter the stack
- Generate `End` events when functions leave the stack
- Events must be chronologically ordered

**Example:**
```
Input:
  Sample 1: [2.4ms] main -> foo -> bar
  Sample 2: [5.0ms] main -> foo -> baz

Output Events:
  Begin 2.4ms main
  Begin 2.4ms foo
  Begin 2.4ms bar
  End   2.4ms bar
  Begin 5.0ms baz
  End   5.0ms baz
  End   5.0ms foo
  End   5.0ms main
```

**Key Insight:** Compare consecutive stacks to find:
- Common prefix (functions still running)
- Functions that exited (generate End events, innermost first)
- Functions that entered (generate Begin events, outermost first)
