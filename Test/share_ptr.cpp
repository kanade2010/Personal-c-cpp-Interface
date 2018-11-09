/*
shared_ptr

shared_ptr允许多个该智能指针共享第“拥有”同一堆分配对象的内存，这通过引用计数（reference counting）实现，会记录有多少个shared_ptr共同指向一个对象，一旦最后一个这样的指针被销毁，也就是一旦某个对象的引用计数变为0，这个对象会被自动删除。
 */


#include <memory>


/*
int main()
{
  std::shared_ptr<int> sp0(new int(2));
            std::shared_ptr<int> sp1(new int(11));
            std::shared_ptr<int> sp2 = sp1;
            printf("%d\n", *sp0);               // 2
            printf("%d\n", *sp1);               // 11
            printf("%d\n", *sp2);               // 11
            sp1.swap(sp0);
            printf("%d\n", *sp0);               // 11
            printf("%d\n", *sp1);               // 2
            printf("%d\n", *sp2);               // 11

            std::shared_ptr<int> sp3(new int(22));
            std::shared_ptr<int> sp4 = sp3;
            printf("%d\n", *sp3);               // 22
            printf("%d\n", *sp4);               // 22
            sp3.reset();                        
            printf("%d\n", sp3.use_count());    // 0
            printf("%d\n", sp4.use_count());    // 1
            printf("%d\n", sp3);                // 0
            printf("%d\n", sp4);                // 指向所拥有对象的地址
            printf("%d\n", *sp4);               // 22
            printf("\n--------------------\n");   // 22

            std::shared_ptr<int> sp5(new int(22));
            std::shared_ptr<int> sp6 = sp5;
            std::shared_ptr<int> sp7 = sp5;
            printf("%d\n", *sp5);               // 22
            printf("%d\n", *sp6);               // 22
            printf("%d\n", *sp7);               // 22
            printf("%d\n", sp5.use_count());    // 3
            printf("%d\n", sp6.use_count());    // 3
            printf("%d\n", sp7.use_count());    // 3
            sp5.reset(new int(33));                        
            printf("%d\n", sp5.use_count());    // 1
            printf("%d\n", sp6.use_count());    // 2
            printf("%d\n", sp7.use_count());    // 2
            printf("%d\n", *sp5);               // 33
            printf("%d\n", *sp6);               // 22
            printf("%d\n", *sp7);               // 22
}

*/
*



int main()
{


  return 0;
}