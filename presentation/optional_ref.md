# `std::optional<T&>`

## we want it

* for a large number of people as "we"
  * including me(Peter): [P2070](https://wg21.link/P2070)
  * JeanHeyd Meneide: [P1175](https://wg21.link/P1175)[P1683](https://wg21.link/P1683)
    * [SanDiego1175](https://wiki.edg.com/bin/view/Wg21sandiego2018/P1175)
  * plus numerous supporters on the mailing list giving a +1 or +100

* any non-standard C++ optional implementation supports lvalue references

> please standardize existing practice

## Why it isn't in the standard yet

> Removed, because couldn't agree on assignment

* Hawaiian sun in Kona?
* Frozen brains in meeting room?
* Design by committee?
* FUD?



see also https://thephd.dev/to-bind-and-loose-a-reference-optional

# What I (Peter) teach

> Get rid of plain pointers

* eliminates chances of pointer arithmetic
* eliminates confusion with plain array
* eliminates confusion about references

> requires an optional for references

> IMO, the Core Guidelines are wrong in their use of "single object pointer" meaning an optional reference.

## Pointer replacement overview

<table style="font-size:60%">
<colgroup>
<col span="3" style="font-size:80%;width: 10%;">
<col span="1" style=";">
<col span="1" style="font-size:60%;width: 35%">
</colgroup>

<tbody>
<tr class="fragment">
<td rowspan="4" style="vertical-align:middle;">
owning `T`{.cpp}
</td>
<td rowspan="2"  style="vertical-align:middle;">non-null</td>
<td style="background-color: #73FA79A0">value</td>
<td style="background-color: #73FA79A0">
`T`{.cpp}
</td>
<td style="font-size:80%; background-color: #73FA79A0">most safe and useful</td>
</tr>
<tr class="fragment">
<td style="vertical-align:middle;">heap</td>
<td  style="vertical-align:middle;">
`unique_ptr<T> const `{.cpp style="font-size:80%"}
</td>
<td style="font-size:80%">

must be init with<br/>`make_unique<T>`{.cpp}

</td>
</tr>
<tr class="fragment">
<td rowspan="2" style="vertical-align:middle; background-color: lightpink;">
nullable
<p style="size:200%;">🦸‍♀️</p>
</td>
<td style="background-color: #EAFDC7A0; vertical-align:middle;">value</td>
<td style="font-size:80%; background-color: #EAFDC7A0; vertical-align:middle;">
`optional<T>`{.cpp}
</td>
<td style="font-size:80%; background-color: #EAFDC7A0">
to denote missing value<br/>
best for return values 
</td>
</tr>
<tr class="fragment">
<td style="vertical-align:middle;">heap</td>
<td style="vertical-align:middle;">
`unique_ptr<T> const `{.cpp style="font-size:80%"}
</td>
<td style="font-size:80%">
`T`{.cpp} can be base class<br/>with `make_uniqe<Derived>`{.cpp}
</td>
</tr>
<tr class="fragment">
<td rowspan="4"  style="vertical-align:middle; background-color: lightpink;">

referring `T`{.cpp}
<p style="size:200%">🦸‍♀️</p>
</td>
<td rowspan="2" style="vertical-align:middle;">non-null</td>
<td  style="background-color: #EAFDC7A0">fixed</td>
<td  style="background-color: #EAFDC7A0">
`T &`{.cpp}
</td>
<td style="font-size:100%; background-color: #EAFDC7A0; color:red">
can dangle
</td>
</tr>
<tr class="fragment">
<td>rebind</td>
<td>
`reference_wrapper<T>`{.cpp}
</td>
<td style="font-size:70%">
assignability with a reference member
</td>
</tr>
<tr class="fragment">
<td rowspan="2" style="vertical-align:middle; background-color: lightpink;">
nullable
<p style="size:200%">🦸‍♀️</p>
</td>
<td  style="background-color: #F9DE96A0; vertical-align:middle;">fixed</td>
<td  style="background-color: #F9DE96A0; font-size: 70%; vertical-align:middle;">
  
`jss::object_ptr<T> const`{.cpp}<br/>
`boost::optional<T&> const`{.cpp}

</td>
<td rowspan="2" style="vertical-align:middle;font-size:70%;background-color: #F9DE96A0">
  
missing in std<br/>
`std::optional`{.cpp} can not do this<br/>
`boost::optional`{.cpp} can<br/>
`object_ptr<T>`{.cpp} by A. Williams
  
</td>
</tr>
<tr class="fragment">
<td  style="background-color: #F9DE96A0; vertical-align:middle;">rebind</td>
<td style="background-color: #F9DE96A0; font-size: 70%; vertical-align:middle;">
`jss::object_ptr<T>`{.cpp}<br/>
`boost::optional<T&>`{.cpp}<br/>
`optional<reference_wrapper<T>>`{.cpp}
</td>
</tr>
</tbody>
</table>

# Handling Assignment Controversy

> cross-type assignment is overused

* general remark by me, old days as an "optimization"
* in the same realm as non-`explicit` constructors or conversion operators

> assigning a reference to an `optional<T&>` rebinds.

* no other semantics is actually reasonable. 

## Why the mess wrt assignment ?

:::{style="font-size:80%;text-align:left;"}

N3406 says it at all:

> These entities [optional references] are surprising to many people because they do not appear to add any more functionality than pointers do.

I say:

> Pointers have a too wide interface (arithmetic!, array semantics, `void*` conversion), optional references remove unsafe operations and provide semantic meaning lacking in a pointer, where it is only given by convention, as in the Core Guidelines

`optional<T&>` 

* type says what it is
* monadic API makes usage superior to `T*`

:::