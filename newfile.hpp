/* 
 * File:   newfile.hpp
 * Author: paradise
 *
 * Created on 3 de septiembre de 2013, 15:57
 */

#ifndef NEWFILE_HPP
#define	NEWFILE_HPP

#include <iostream>                              

template<typename T>
struct base
{
    void foo() const
    {
        base<T>* this_copy = const_cast<base<T>*>(this);
        
        static_cast<T*>( this_copy )->foo();
    }
    
    void execute_bar() const
    {
        base<T>* this_copy = const_cast<base<T>*>(this);
        
        static_cast<T*>( this_copy )->bar();
    }
    
    void execute_quux() const
    {
        base<T>* this_copy = const_cast<base<T>*>(this);
        
        static_cast<T*>( this_copy )->quux();
    }
};

struct Foo : public base<Foo>
{
    void foo() const
    {
        std::cout << "Foo foo" << std::endl;
    }
    
    void bar() const
    {
        std::cout << "Foo bar" << std::endl;
    }
    
    void quux()
    {
        std::cout << "Foo quux" << std::endl;
    }
};

struct Bar : public base<Bar>
{
    void foo()
    {
        std::cout << "Bar foo" << std::endl;
    }
    
    void bar()
    {
        std::cout << "Bar bar" << std::endl;
    }
    
    void quux()
    {
        std::cout << "Bar quux" << std::endl;
    }
};

struct Quux : public base<Quux>
{
    void foo()
    {
        std::cout << "Quux foo" << std::endl;
    }
    
    void bar()
    {
        std::cout << "Quux bar" << std::endl;
    }
    
    void quux()
    {
        std::cout << "Quux quux" << std::endl;
    }
};



#endif	/* NEWFILE_HPP */

