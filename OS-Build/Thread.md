## 线程



### PCB相关

> 定义

```
typedef void thread_fun(void*);

enum task_status{
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_WAITING,
    TASK_HANGING,
    TASK_DIED
}

/****** 中断栈 *******
* 用于中断时保护程序的上下文环境
* 中断时按此压入上下文寄存器。
* intr_exit中借此恢复上下文环境
* 此栈在线程自己的内核栈中位置固定，所在页的最顶端
*/
struct intr_stack{
  uint vec_no ; 压入中断号
  uint edi ;
  uint esi ;
  uint ebp ;
  uint esp_dummy ; //虽然压入了sp，但他是不断变化的，所以可以忽略
  uint ebx ;
  uint edx ;
  uint ecx ;
  uint eax ;
  uint gs ;
  uint fs ;
  uint es ;
  uint ds ;

  // 进入高特权级时压入
  uint err_code ;
  void (*eip)(void) ;
  uint cs;
  uint eflags;
  void *esp;
  uint ss;
};

/****** 线程栈 *******
*  线程自己的栈
*/
struct thread_stack{
  uint edp;
  uint ebx;
  uint edi;
  uint esi;

  //第一次执行时，eip指向带调用函数
  void (*eip)(thread_func* func,void* func_arg);

  // 当第一次调入cpu
  void (*unused_retaddr); // 占位，充当返回地址
  thread_func* function; // 调用的函数名
  void *func_arg;  //  调用的所需参数
};


/* PCB */
struct task_struct{
  uint * self_stack; // 指针，指向所在页顶端
  enum task_status status ;   // status
  uint priority ; //
  char name[16] ;
  uint stack_magic ;
};
```

> 实现

```
static void kernel_thread(thread_func* function,void* func_arg){
  function(func_arg);
}

void thread_create(struct task_struct* pthread,thread_func function,void *func_arg){
  // 预留中断栈的空间
  pthread->self_kstack-=sizeof(struct intr_stack);
  // 预留线程栈空间
  pthread->self_kstack-=sizeof(struct thread_stack);

  struct thread_stack *kthread_stack = (struct thread_stack*)pthread->self_kstack;
  kthread_stack->eip = kernel_thread ;
  kthread_stack->function = function ;
  kthread_stack->func_arg = func_arg ;
  kthread_stack->ebp =kthread_stack->ebx  =kthread_stack->esi  =kthread_stack->edi  = 0;
}

/*初始化线程信息*/
void init_thread(struct task_struct* pthread,char* name,int prio){
  memset(pthread,0,sizeof(*pthread));
  strcpy(pthred->name,name);
  pthread->status = TASK_RUNNING ;
  pthread->priority = prio ;
}

/*创建线程*/
struct task_struct* thread_start(char *name,int prio,thread_func function,void *func_arg){
  struct task_struct* thread = get_page(1);
  init_thread(thread,name,prio);
  thread_create(thread,function,func_arg);
  return thread ;
}

```
