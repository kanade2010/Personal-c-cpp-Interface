
/*
weak_ptr

weak_ptr是为配合shared_ptr而引入的一种智能指针来协助shared_ptr工作，它可以从一个shared_ptr或另一个weak_ptr对象构造，它的构造和析构不会引起引用计数的增加或减少。没有重载 * 和 -> 但可以使用lock获得一个可用的shared_ptr对象

weak_ptr的使用更为复杂一点，它可以指向shared_ptr指针指向的对象内存，却并不拥有该内存，而使用weak_ptr成员lock，则可返回其指向内存的一个share_ptr对象，且在所指对象内存已经无效时，返回指针空值nullptr。

注意：weak_ptr并不拥有资源的所有权，所以不能直接使用资源。
可以从一个weak_ptr构造一个shared_ptr以取得共享资源的所有权。

 */
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>

void check(std::weak_ptr<int> &wp)
{
    std::shared_ptr<int> sp = wp.lock(); // 转换为shared_ptr<int>
    if (sp != nullptr)
    {
        std::cout << "still: " << *sp << std::endl;
    } 
    else
    {
        std::cout << "still: " << "pointer is invalid" << std::endl;
    }
}


void mytest()
{
    std::shared_ptr<int> sp1(new int(22));
    std::shared_ptr<int> sp2 = sp1;
    std::weak_ptr<int> wp = sp1; // 指向shared_ptr<int>所指对象

    std::cout << "count: " << wp.use_count() << std::endl; // count: 2
    std::cout << *sp1 << std::endl; // 22
    std::cout << *sp2 << std::endl; // 22
    check(wp); // still: 22
    
    sp1.reset();
    std::cout << "count: " << wp.use_count() << std::endl; // count: 1
    std::cout << *sp2 << std::endl; // 22
    check(wp); // still: 22

    sp2.reset();
    std::cout << "count: " << wp.use_count() << std::endl; // count: 0
    check(wp); // still: pointer is invalid

    return;
}

int main()
{
    mytest();

    system("pause");
    return 0;
}