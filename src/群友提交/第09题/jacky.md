在 `Y<T>::t2()` 中，`f` 不是一个待决名（*dependent name*），而 `X<T>` 则是待决名。<br/>
在查找 `f`（在 `Y<T>::t2()` 中）时，因为它不是待决名，所以在模板实参未知的时候就会进行查找，因此不会查找到待决名 `X<T>`，就无法找到 `X<T>::f()`，最终找到 `::f()`。<br/>
而在 `t2()` 中，由于 `this` 始终是一个隐式的待决名，所以会推迟到模板实参已知时查找，从而找到待决名 `Y<T>::t2()`。

参考资料：
<br/>
<https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members>
