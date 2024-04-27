# OS-MinorHW1

## 1. 作业介绍

### 1.1 优先级调度

我们实现了基于优先级的 process scheduler，具体来说，我们首先在 `"proc.h"` 中加入了如下字段：

```c++
struct proc_queue {
    int size;
    int id;                    // there are 0, 1, 2 three queues
    int max_slice;             // maximum time slice one process can get
    struct proc *queue[64];    // maximum 64 processes in one queue
};

struct proc {
    // omitted
    int priority;                // Process priority
    int time_slice;              // Process time slice remaining
    struct proc_queue *queue;    // Process queue
}
```

我们维护了 3 个 `proc_queue` 用于调度，调度规则如下 (credit to @peterzheng98):

> 调度器拥有3个调度队列。所有进程在创建时被调度到队列1中，且分配2个时间片。默认的优先级别为0。
> - 当队列1中的所有进程时间片用尽时，进程会被放到队列2中，且分配10个时间片。
> - 当队列2中的所有进程时间片用尽时，检查队列3中是否存在进程。如果存在，则执行队列3且总共占用10个时间片。
> - 每个队列内都采用RR调度。任意时刻进程被设置为>0的优先级时立即加入队列1，<0的优先级则立即加入队列3。通过优先级加入的队列持有的时间片是他们的优先级数值的绝对值，原有的时间片被丢弃。

此外，我们将优先级调度的接口暴露给用户，并实现了 `set_priority` 和 `get_priority` 两个 system call。

```c++
long long proc_set_priority(int t); // set priority and change schedule queue
long long proc_get_priority();      // get priority for users
void proc_yield();                  // use yield() to test correctness
```

### 1.2. 进程级别 ACL (Access Control List)

我们考虑抽象资源如下：

```c++
uint64 kernel_object[MAX_OBJ];
struct spinlock object_lock;
```

我们用 `acl_control` 变量来维护每个抽象资源的访问权限，用于细粒度地进程访问控制，并实现了如下系统调用：

```c++
uint64 read_object(int idx);                // read object if we have the permission
uint64 write_object(int idx, uint64 value); // write object if we have the permission
uint64 read_acl();                          // read the access-control-list
uint64 write_acl(uint64 value);             // write the access-control-list
```

## 2. 测试

已通过 `usertests`，证明进程调度的正确性和鲁棒性
已通过 @peterzheng98 设计的 `aclobj`（见 `./user/aclobj.c`）