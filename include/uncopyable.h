/*
 *	防止对象拷贝
 *	Author:daihongtian
 */

#pragma once

class Uncopyable
{
protected:
    Uncopyable() {}

    ~Uncopyable() {}
private:
    Uncopyable(const Uncopyable&);				//防止拷贝构造

    Uncopyable& operator=(const Uncopyable &);	//防止赋值不实现
};

