# C++类内存结构模型

## 内存分区

内存全局数据区，代码区，栈区，堆区。

定义一个类

>类的成员函数被放在代码区
>
>类的静态成员变量被放在全局数据区（不占用类的存储空间）
>
>非静态成员在类的实例内，实例在栈区或者堆区
>
>虚函数指针，虚基类指针在类的实例内，实例在堆区或者栈区

## 内存布局实例

### 普通类（不携带静态成员，也没有继承关系）

>```c++
>class A1
>{
>public:
>	void func1() {};
>public:
>	int a1;
>	char a2;
>	char a3;
>};
>
>class A2
>{
>public:
>	void func2() {};
>public:
>	char a1;
>	int a2;
>	char a3;
>};
>```
>
>>class A1	size(8):
>>	+---
>> 0	| a1
>> 4	| a2
>> 5	| a3
>>  	| <alignment member> (size=2)
>>	+---
>>class A2	size(12):
>>	+---
>> 0	| a1
>>  	| <alignment member> (size=3)
>> 4	| a2
>> 8	| a3
>>  	| <alignment member> (size=3)
>>	+---
>
>A1类携带3个成员变量及1个成员函数，成员函数不占用内存，在x86平台下，整型变量a1（占用4个字节），字符型变量a2（占用一个字节），a3（占用一个字节）, 总共占用内存为 8 字节。因为内存是4字节对齐的，2个字符变量占用2字节被补齐为4字节。
>
>A2类携带和A1类相同的变量，只不过调整了下顺序，第1个为字符变量（占用1个字节，补齐为4字节），第2个为整型变量（占用4字节），第3个为字符变量（占用1字节补齐为4字节）

### 类（携带静态成员变量）

>```c++
>class B
>{
>public:
>	void func1();
>public:
>	static int b1;
>	int b2;
>};
>```
>
>>该类的类名为B。用简图表示该类内存布局：
>>
>>class B	size(4):
>>	+---
>>0	| b2
>>
>>​    +---
>
>B类携带静态整型成员变量b1和整型变量b2，静态整型变量b1不占用类的内存，整型变量占用4字节的内存，因而B类占用内存为4字节。

### 类（携带虚函数，并继承）

>```c++
>class C1
>{
>public:
>	void func1();
>	virtual void func2();
>public:
>	int a1;
>};
>
>class C2 : public C1
>{
>public:
>	void func2() override;
>};
>
>class C3
>{
>public:
>	virtual void func1();
>	virtual void func2();
>public:
>	int a1;
>};
>```
>
>
>
>>class C1	size(8):
>>	+---
>> 0	| {vfptr}
>> 4	| a1
>>	+---
>>C1::$vftable@:
>>	| &C1_meta
>>	|  0
>> 0	| &C1::func2
>>C1::func2 this adjustor: 0
>>
>>
>>
>>class C2	size(8):
>>	+---
>> 0	| +--- (base class C1)
>> 0	| | {vfptr}
>> 4	| | a1
>>	| +---
>>	+---
>>C2::$vftable@:
>>	| &C2_meta
>>	|  0
>> 0	| &C2::func2
>>C2::func2 this adjustor: 0
>>
>>
>>
>>class C3	size(8):
>>	+---
>> 0	| {vfptr}
>> 4	| a1
>>	+---
>>C3::$vftable@:
>>	| &C3_meta
>>	|  0
>> 0	| &C3::func1
>> 1	| &C3::func2
>>C3::func1 this adjustor: 0
>>C3::func2 this adjustor: 0
>
>类C1携带整型成员变量a1（占用4字节内存），普通成员函数func1，虚成员函数func2（一旦一个类对象携带虚函数，则该类有一个虚函数指针，在x86平台下，该指针的大小为4字节）故而类C1的大小为8字节。虚函数指针指向一个虚函数表（是一个数组），类C1中有多少个虚函数，虚函数表中就有多少个虚函数（数组中就有多少个虚函数，按照定义虚函数的顺序进行排列）
>
>类C2继承类C1，虚函数func2继承于类C1，C2的虚函数表的第1个元素即为C2:func2
>
>类C3携带2个虚函数，虚函数表中有2个元素，第1个元素为C3:func1 ，第2个元素为C3:func2

### 类（携带虚函数，并多继承）

>```
>class D1
>{
>public:
>	virtual void func1();
>};
>
>class D2
>{
>public:
>	virtual void func2();
>};
>
>class D3 : public D1, D2
>{
>};
>```
>
>>>class D1   size(4):
>>>
>>>  +---
>>>
>>> 0  | {vfptr}
>>>
>>>  +---
>>>
>>>D1::$vftable@:
>>>
>>>  | &D1_meta
>>>
>>>  |  0
>>>
>>> 0  | &D1::func1
>>>
>>>D1::func1 this adjustor: 0
>>>
>>>class D2   size(4):
>>>
>>>  +---
>>>
>>> 0  | {vfptr}
>>>
>>>  +---
>>>
>>>D2::$vftable@:
>>>
>>>  | &D2_meta
>>>
>>>  |  0
>>>
>>> 0  | &D2::func2
>>>
>>>D2::func2 this adjustor: 0
>>>
>>>class D3   size(8):
>>>
>>>  +---
>>>
>>> 0  | +--- (base class D1)
>>>
>>> 0  | | {vfptr}
>>>
>>>  | +---
>>>
>>> 4  | +--- (base class D2)
>>>
>>> 4  | | {vfptr}
>>>
>>>  | +---
>>>
>>>  +---
>>>
>>>D3::$vftable@D1@:
>>>	| &D3_meta
>>>	|  0
>>> 0	| &D1::func1
>>>D3::$vftable@D2@:
>>>	| -4
>>> 0	| &D2::func2
>>
>>类D3继承D1和D2，D3中含有2个虚函数指针，占用大小为8字节，虚函数指针1指向D1的虚函数表，虚函数D2指向D2的虚函数表。（这部分可能理解有误，如有错误，希望帮忙指出）