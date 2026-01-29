### Implement a Skip List

**Your Task**
Build a probabilistic data structure that allows O(log n) search, insert, and delete.

**How Skip Lists Work:**
- Multiple layers of linked lists
- Bottom layer has all elements
- Higher layers skip elements (probabilistic)
- Each node has random height (coin flip: 50% chance to go higher)

**Requirements:**
- `insert(val)`: Add value maintaining sorted order
- `search(val)`: Return true if exists
- `erase(val)`: Remove value if exists
- Expected O(log n) time for all operations

**Example:**
```
Level 3:  1 --------> 9
Level 2:  1 --> 4 --> 9
Level 1:  1 --> 4 --> 7 --> 9
Level 0:  1 --> 4 --> 6 --> 7 --> 9
```
