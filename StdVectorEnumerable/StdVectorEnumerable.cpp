// StdVectorEnumerable.cpp : main project file.

#include "stdafx.h"
#include <vector>

using namespace System;
using namespace System::Collections::Generic;

template <class N, class T>
ref class StdVectorEnumerable : public IEnumerable<N>
{
private:
    ref class Iterator : public IEnumerator<N>
    {
    private:
        typedef typename std::vector<T>::iterator MyIter;
        MyIter* _it;
        std::vector<T>& _vec;

    public:
        Iterator(std::vector<T>& vec) : _vec(vec), _it(NULL) {}
        ~Iterator() { delete _it; }
        !Iterator() { delete _it; }

        property N Current
        {
            virtual N get() { return convert(**_it); }
        }

        virtual System::Object^ old_Current_get() sealed = System::Collections::IEnumerator::Current::get
        { return Current; }

        virtual bool MoveNext()
        {
            if (_it == NULL)
            {
                _it = new MyIter(_vec.begin());
                return (*_it) != _vec.end();
            }

            return ++(*_it) != _vec.end();
        }

        virtual void Reset() { }

    private:
        N convert(T& item);
    };

    std::vector<T>* _vec;

public:
    StdVectorEnumerable()
    {
        _vec = new std::vector<T>();
    }

    ~StdVectorEnumerable()
    {
        cleanup();
    }

    !StdVectorEnumerable()
    {
        cleanup();
    }

    virtual IEnumerator<N>^ GetEnumerator()
    {
        return gcnew Iterator(*_vec);
    }

    virtual System::Collections::IEnumerator^ old_GetEnumerator() = System::Collections::IEnumerable::GetEnumerator
    {
        return GetEnumerator();
    }

    std::vector<T>& AsVector()
    {
        return *_vec;
    }

private:
    void cleanup()
    {
        delete _vec;
    }
};

template<>
System::String^ StdVectorEnumerable<System::String^, std::string>::Iterator::convert(std::string& item)
{
    return gcnew System::String(item.c_str());
}

#define BEGIN_INTERFACE( NAME ) \
    class NAME { public:        \
        NAME () {}              \
        virtual ~ NAME () {}
#define END_INTERFACE( NAME )   };
#define PURE( SIGNATURE, ... )   virtual SIGNATURE (__VA_ARGS__) = 0;

BEGIN_INTERFACE( IMine )
    PURE( void Stuff , int, bool )
END_INTERFACE( IMine )

int main(array<System::String ^> ^args)
{
    StdVectorEnumerable<System::String^, std::string>^ x = gcnew StdVectorEnumerable<System::String^, std::string>();

    std::vector<std::string>& vec = x->AsVector();
    vec.push_back("a");
    vec.push_back("b");
    vec.push_back("c");

    for each (System::String^ item in x)
        Console::WriteLine(item);

    Console::WriteLine(L"Hello World");
    Console::Read();
    return 0;
}
