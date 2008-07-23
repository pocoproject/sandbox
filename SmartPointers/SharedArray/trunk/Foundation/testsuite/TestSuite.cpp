//
// TestSuite.cpp
//
// $Id: //poco/1.3/Foundation/testsuite/src/SharedArrayTest.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#include "TestSuite.h"

#include <Poco/SharedArray.h>
#include <Poco/Mutex.h>

#include <assert.h>
#include <iostream>
#include <vector>

using Poco::SharedArray;
using Poco::NullPointerException;

namespace
{
    class TestObject
    {
    public:

		TestObject() : _data(0){
            Poco::FastMutex::ScopedLock lock(_mutex);
            ++_count;
            std::cout << "current counter : " << _count << "\n";
		}

        TestObject(const int& data) : _data(data)
        {
            Poco::FastMutex::ScopedLock lock(_mutex);
            ++_count;
            std::cout << "current counter : " << _count << "\n";
        }

        virtual ~TestObject()
        {
            Poco::FastMutex::ScopedLock lock(_mutex);
            --_count;
            std::cout << "current counter : " << _count << "\n";
        }

        void set(const int& data)
        {
            _data = data;
        }

        int& data()
        {
            return _data;
        }

        static int count()
        {
            return _count;
        }

    private:
        int _data;
        static int _count;

        Poco::FastMutex _mutex;
    };

    int TestObject::_count = 0;
}

void testConstruction()
{

    // construct from explicit raw pointer to dynamical allocated array
    {        
        SharedArray<TestObject> ptr2(new TestObject[10]);
        poco_assert(ptr2.get() != 0);
        poco_assert(ptr2);
        poco_assert(!ptr2 == false);
        poco_assert(TestObject::count() == 10);

        // the following code wont compile
        TestObject* p = new TestObject[10];
        SharedArray<TestObject> ptr3(p);
    }

    poco_assert(TestObject::count() == 0);

    // copy construction 
    {        
        SharedArray<TestObject> ptr3(new TestObject[10]);
        poco_assert(ptr3.get() != 0);
        poco_assert(TestObject::count() == 10);

        SharedArray<TestObject> ptr4(ptr3);
        poco_assert(TestObject::count() == 10);
    }
    poco_assert(TestObject::count() == 0);

}

void testAssignment()
{
    poco_assert(TestObject::count() == 0);

    {
        
        SharedArray<TestObject> ptr1(new TestObject[10]);
        poco_assert(ptr1.get() != 0);
        poco_assert(TestObject::count() == 10);

        SharedArray<TestObject> ptr2 = ptr1;

        poco_assert(TestObject::count() == 10);

    }

    poco_assert(TestObject::count() == 0);

    // note we use explicit constructor for raw pointer, the following wont compile
    // {        
    //    TestObject*p = new TestObject[10];        
    //    SharedArray<TestObject> ptr1 = p;
    // }


}

void testSwap()
{
    poco_assert(TestObject::count() == 0);

    {
        
        SharedArray<TestObject> ptr1(new TestObject[10]);

        // before swap
        poco_assert(ptr1.get() != 0);
        poco_assert(TestObject::count() == 10);

        {
            SharedArray<TestObject> ptr2(new TestObject[20]);
            ptr2.swap(ptr1);
            poco_assert(TestObject::count() == 30);     
            poco_assert(ptr2.get() != 0);
        }

        // after swap
        poco_assert(ptr1.get() != 0);
        poco_assert(TestObject::count() == 20);

    }

    poco_assert(TestObject::count() == 0);

}

void testElementAccess()
{
    poco_assert(TestObject::count() == 0);

    {
        
        SharedArray<TestObject> ptr1(new TestObject[10]);

        for(int i = 0; i<10; i++){
            ptr1[i].set(i);
        }

        for(int i = 0; i<10; i++){
            poco_assert(i == ptr1[i].data());
        }        

    }

    poco_assert(TestObject::count() == 0);

}


void testAddressComparison()
{
    poco_assert(TestObject::count() == 0);

    {
        
        SharedArray<TestObject> ptr1(new TestObject[10]);
        SharedArray<TestObject> ptr2(new TestObject[10]);
        SharedArray<TestObject> ptr3(ptr1);

        poco_assert(ptr1);
        poco_assert(ptr2 != ptr1);
        poco_assert(ptr3 == ptr1);

        if(ptr1.get() > ptr2.get()){
           poco_assert(ptr1 > ptr2);
        }
        if(ptr1.get() >= ptr2.get()){
            poco_assert(ptr1 >= ptr2);
        }
        if(ptr1.get() < ptr2.get()){
            poco_assert(ptr1 < ptr2);
        }
        if(ptr1.get() <= ptr2.get()){
            poco_assert(ptr1 <= ptr2);
        }


    }

	// test operator(), operator!()  operators and free operators 
	{
		TestObject * rawPointer = 0;
        SharedArray<TestObject> ptr5(rawPointer = new TestObject[10]);
		poco_assert(TestObject::count() == 10);
        poco_assert(ptr5);
        poco_assert(!!ptr5);
        poco_assert(rawPointer == ptr5);

        SharedArray<TestObject> ptr6(ptr5);
		poco_assert(TestObject::count() == 10);
        poco_assert(ptr6);
        poco_assert(!!ptr6);
        poco_assert(ptr6 == ptr5);
        poco_assert(rawPointer == ptr6);

		TestObject* rawPointer2 = rawPointer+1;
        poco_assert(rawPointer2 > ptr6);

	}

    poco_assert(TestObject::count() == 0);

    poco_assert(TestObject::count() == 0);

}

template<class T>
struct logDeleter
{
	void operator()(T* p)
	{
		std::cout << "log array deleter\n";
		delete [] p;
	}
};

template<class T>
void freeFuncDeleter(T* p)
{
	std::cout << "free function deleter\n";
	delete [] p;
};

void testCustomDeleter()
{
    poco_assert(TestObject::count() == 0);

    {

		SharedArray<TestObject> functorDeleter(new TestObject[10], logDeleter<TestObject>() );
        poco_assert(TestObject::count() == 10);

		SharedArray<TestObject> functionDeleter(new TestObject[10], freeFuncDeleter<TestObject> );
        poco_assert(TestObject::count() == 20);

    }

    poco_assert(TestObject::count() == 0);

    {

		SharedArray<TestObject> functorDeleter(new TestObject[10], logDeleter<TestObject>() );
        poco_assert(TestObject::count() == 10);

		{

			SharedArray<TestObject> functionDeleter(new TestObject[20], freeFuncDeleter<TestObject> );
			poco_assert(TestObject::count() == 30);

			SharedArray<TestObject> temp = functionDeleter;
			functionDeleter = functorDeleter;
			functorDeleter = temp;

		}

		poco_assert(TestObject::count() == 20);
    }
	
	poco_assert(TestObject::count() == 0);

}

void testStlContainers()
{
    poco_assert(TestObject::count() == 0);

    {
        
        typedef std::vector<SharedArray<TestObject> >             TContainer;
        typedef std::vector<SharedArray<TestObject> >::iterator   TIter;

        TContainer container;

        container.push_back(SharedArray<TestObject> (new TestObject[10]));
        container.push_back(SharedArray<TestObject> (new TestObject[10]));
        container.push_back(SharedArray<TestObject> (new TestObject[10]));           

        poco_assert(TestObject::count() == 30);

        for(TIter it = container.begin(); it != container.end(); ++it){
            (*it)[0].set(0);
        }

        poco_assert(container[0][0].data() == 0);
        poco_assert(container[1][0].data() == 0);
        poco_assert(container[2][0].data() == 0);

    }

    poco_assert(TestObject::count() == 0);

}


int main()
{
    testConstruction();
    testAssignment();
    testSwap();
    testElementAccess();
    testAddressComparison();
    testCustomDeleter();
    testStlContainers();
}