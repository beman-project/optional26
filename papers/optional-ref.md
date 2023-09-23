---
title: std::optional<T&>
document: D0000R0
date: today
audience:
  - None
author:
  - name: Steve Downey
    email: <sdowney@gmail.com>, <sdowney2@bloomberg.net>

toc: false
---

# Abstract
An optional over a reference.

# Comparison table

::: cmptable

### Before
```c++
std::shared_ptr<Cat> cat = find_cat("Fido");
// or
std::map<std::string, Cat>::iterator = find_cat("Fido");
// or
Cat* cat = find_cat("Fido");
```
### After
```c++
std::optional<Cat&> cat = find_cat("Fido");
```

:::

# Problem to be solved
Optionals holding references are common other than in the standard libary's implementation. The research in [@P1683R0] shows conclusively that rebind semantics are the only safe semantic as assign through on engaged is too bug-prone. Implementations that attempt assign-through are abandoned. The standard library should follow existing practice and supply an optional<T&> that rebinds on assignment.
