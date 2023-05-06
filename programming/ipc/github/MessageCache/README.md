# Message Cache
This is a high performance message cache in memory.  
The message should be sequenced and binary.

### Get Started

You can buffer/recovery the message as follows:
```c++
// Firstly, create the cache instance.
// The defaule templete paramter is uint32_t. it is the type of the sequence number.
// bufferSize:
//   It specify how many message need be buffered.
MessageCache<> cache{bufferSize};

// The defaule templete paramter is uint32_t, it should must be same as the value in MessageCache 
// seq:
//   It is the message sequence, should start from 1 and continuous,
//   If there is a gap. the cache will fill the missed message with empty entity. 
// buf:
//   The address of the orignal massage. it is uint8_t*.
// size:
//   The size of the message.
Entity<> msg(seq, buf, size);

// Add the message to the cache.
// Please note you should use std::move, after this function, do not use the msg variable any more.
// It should be invalid.
cache.addMessage(std::move(msg));

// To get a message from the cache, call getMessage and tell the cache with the specified sequence
Entity<> recovery;
cache.getMessage(seq, recovery);

```
