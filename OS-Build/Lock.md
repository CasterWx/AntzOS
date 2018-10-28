## 锁

#### 信号量与锁的实现思想

> sync.h
```c
    // 信号量
    struct semaphore {
        uint value ;
        struct list waiters ;
    }
    // 锁
    struct lock {
        struct task_struct* holder ; // 锁持有者
        struct semaphore semaphore ;
        uint holder_num ;
    }
```

> sync.c
```c
    void sema_init(struct semaphore *sema,uint value){
        sema->value = value ;
        list_init(&sema->waiters);
    }
    void lock_init(struct lock *plock){
        plock->holder = NULL ;
        plock->holder_num = 0 ;
        sema_init(&plock->semaphore,1);
    }
    void sema_down(struct semaphore *psema){
         // 关中断操作
         while(psema->value==0){
            // 如果当前线程已经在waiters队列中
            ASSERT(!elem_find(&psema->waiters,running_thread()->general_tag));
            if(elem_find(&psema->waiters,running_thread()->general_tag)){
                // error
            }
            // 加入等待队列
            append_list(&psame->waiters,&running_thread()->general_tag);
            thread_block(TASK_BLOCKED); //阻塞自己
         }
         psame->value -- ;
         ASSERT(psame->value==0);
         // 回复之前的中断状态
    }
    void sema_up(struct semaphore *psema){
         // 关中断操作
         ASSERT(psema->value==0);
         if(!list_empty(&psame->waiters)){
            struct task_struct* thred_blocked = elem2entery(struct task_struct,general_tag,list_pop(&psame->waiters));
            thread_unblock(thread_blocked);
         }
         psema->value ++ ;
         ASSERT(psema->value==1);
         // 回复之前的中断状态
    }
    void lock_acquice(strucr lock* plock){
        if(plock->holder!=running_thread()){
            sema_down(&plock->sema);
            plock->holder = running_thread() ;
            ASSERT(plock->holder_num==0);
            plock->holder_num = 1 ;
        }else{
            plock->holder_num++ ;
        }    
    }
    void lock_release(struct lock* plock){
        // 锁的拥有者
        ASSERT(plock->holder==running_thread());
        if (plock->holder_num>1){
            plock->holder_num -- ;
            return ;
        }
        ASSERT(plock->holder_num==1);
        plock->holder = NULL ;
        plock->holder_num = 0 ;
        sema_up(&plock->sema) ;
        // 先释放锁再进行信号量增加
    }
```
