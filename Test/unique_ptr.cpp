/*
ique_ptr持有对对象的独有权，同一时刻只能有一个unique_ptr指向给定对象（通过禁止拷贝语义、只有移动语义来实现）。

unique_ptr指针本身的生命周期：从unique_ptr指针创建时开始，直到离开作用域。

离开作用域时，若其指向对象，则将其所指对象销毁(默认使用delete操作符，用户可指定其他操作)。
 */

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>


void mytest()
{
    std::unique_ptr<int> up1(new int(11));   // 无法复制的unique_ptr
    //unique_ptr<int> up2 = up1;        // err, 不能通过编译
    std::cout << *up1 << std::endl;   // 11

    std::unique_ptr<int> up3 = std::move(up1);    // 现在p3是数据的唯一的unique_ptr

    std::cout << *up3 << std::endl;   // 11
    //std::cout << *up1 << std::endl;   // err, 运行时错误
    up3.reset();            // 显式释放内存
    up1.reset();            // 不会导致运行时错误
    //std::cout << *up3 << std::endl;   // err, 运行时错误

    std::unique_ptr<int> up4(new int(22));   // 无法复制的unique_ptr
    up4.reset(new int(44)); //"绑定"动态对象
    std::cout << *up4 << std::endl; // 44

    up4 = nullptr;//显式销毁所指对象，同时智能指针变为空指针。与up4.reset()等价

    std::unique_ptr<int> up5(new int(55));
    int *p = up5.release(); //只是释放控制权，不会释放内存
    std::cout << *p << std::endl;
    //cout << *up5 << endl; // err, 运行时错误
    delete p; //释放堆区资源

    return;
}

int main()
{
    mytest();

    system("pause");
    return 0;
}