# Infix2postfix
## Supported operators:
- Addition `+`
- Subtraction `-`
- Multiplication `*`
- Division `/`
- Modulo division `%`
- Exponentiation `^`
- Unary minus `-`
- Brackets `()`

## Supported math functions:
| Function name              | Operation                       |
| -------------------------- | ------------------------------- |
| <b>Roots</b>               |                                 |
| `sqrt`                     | Square root                     |
| `cbrt`                     | Cube root                       |
| <b>Trigonometric functions</b> |                             |
| `cos`                      | Cosine                          |
| `sin`                      | Sine                            |
| `tg/tan`                   | Tangent                         |
| `arccos/acos`              | Arccosine                       |
| `arcsin/asin`              | Arcsine                         |
| `arctg/atan`               | Arctangent                      |
| <b>Logarithms</b>          |                                 |
| `ln`                       | Natural logarithm               |
| `log2`                     | Logarithm base 2                |
| `log10`                    | Logarithm base 10               |
| <b>Other</b>               |                                 |
| `abs`                      | Absolute value                  |
| `ceil`                     | Round to smallest integral value|
| `floor`                    | Round to largest integral value |
| `round`                    | Round to integral value         |

## Syntax rules:
- Floating-point numbers can be written both with a point and a comma
- New-line symbol interpreted as the end of expression
- Function call looks like `cos(2*PI)`
- Supports variables
- Supports `pi` and `e` constants with any case
- Supports implicit multiplication of a number by a bracket/function/variable. Supports impilicit multiplication of a variable by a bracket/function
- There can be a different amount of whitespace characters between numbers and operators (`'\ '` or `'\t'`)

## LL(1) GRAMMAR:
`S` -> `S '+' A` | `S '-' A` | `A`  
`A` -> `A '*' B` | `A '/' B` | `A '%' B` | `B`  
`B` -> `'-' C` | `C` 
`C` -> `C '^' factor` | `C '^' '-' factor` | `factor`  
`factor` -> `'num'` | `'(' S ')'` | `'var'` | `'funcName' '(' S ')'`  

## GRAMMAR WITHOUT LEFT RECURSION:
`S` -> `A s`  
`s` -> `'+' A s` | `'-' A s` | `eps`  
`A` -> `B a`  
`a` -> `'*' B a` | `'/' B a` | `'%' B a` | `eps`  
`B` -> `'-' C` | `C`  
`C` -> `factor c`  
`c` -> `'^' factor c` | `'^' '-' factor c` | `eps`  
`factor` -> `'num'` | `'(' S ')'` | `'var'` | `'funcName' '(' S ')'`  


## Works with [postfix-calculator](https://github.com/evgeny-net-x/Postfix-calculator).
Example: `echo "2 * (3 + 2)^2" | ./infix2postfix | ./postfixCalc`.
