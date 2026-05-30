# C Style Guide
Types use PascalCase:
```c
typedef struct CacheSim CacheSim;
typedef enum TraceOp TraceOp;
```
Functions use the same casing whether public or private. If a type name appears in the function name, keep it exactly as the type name with no added underscores. Use underscores only between other word segments:
```c
CacheSim* CacheSim_Create(CacheConfig config);
void CacheSim_Destroy(CacheSim* cache_sim);
```
If the type name is intentionally lowercase, keep that lowercase type prefix:
```c
Error tcp_Connect(tcp_Connection* connection);
```
Variables use lower snake case.
Pointers bind to the type:
```c
CacheSim* cache_sim;
```
Macros and enum values use upper snake case:
```c
#define CACHE_LINE_SIZE 64
typedef enum TraceOp
{
    TRACE_OP_READ = 0,
    TRACE_OP_WRITE = 1
} TraceOp;
```
Opening braces go on their own line:
```c
int main(void)
{
    return 0;
}
```
Use early returns for error handling. Heap-owned objects use `Create` / `Destroy`, caller-owned objects use `Init` / `Deinit`, and fallible APIs should prefer:
```c
Error Function(...);
```