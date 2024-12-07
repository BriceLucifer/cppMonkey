# cpp interpreter
1. token &#10004;
2. lexer &#10004;
3. parser &#10004;
4. ast &#10004;
5. object &#10004;
6. environment (unifinish stuff)
## parser
```interpreter in c++
$ Welcome to the simple Interpreter in C++.
$ Enter expressions or commands.
>> let x = 1 * 2 * 3 * 4 * 5
let x = ((((1 * 2) * 3) * 4) * 5);
>> x * y / 2 + 3 * 8 - 123
((((x * y) / 2) + (3 * 8)) - 123)
>> true == false
(true == false)
>>
```
7. evaluator ?
// 需要修改一下node 增加类型识别