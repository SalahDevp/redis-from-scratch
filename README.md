# Redis From Scratch in c++

this project contains:

- tcp server using socket programming.
- async io and event loop.
- a parser to parse commmands.
- key value in memory database.
- a command line tool to interact with the server.

### The Protocol

#### Overview

the protocal uses tcp and listens on port 6379 by default.

#### Message Structure

**request**:

```
+-----+---------+
| len | payload |
+-----+---------+
```

`len`: 32bit unsigned int
`payload`: string

**response**:

```
+--------+-----+---------+
| status | len | payload |
+--------+-----+---------+
```

`status`: 16bit unsigned int
`len`: 32bit unsigned int
`payload`: string

## References

- https://redis.io/docs/latest/develop/reference/protocol-spec/
- https://www.ibm.com/docs/en/i/7.4?topic=designs-using-poll-instead-select
