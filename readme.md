# xlsx-util

This is a very basic package which allows us to use the native library [xlsxio](https://github.com/brechtsanders/xlsxio) to read the data from an xlsx file.

We hope to provide more functionality in the future, but for now all you can do is extract the data from the first sheet of an xlsx file.

This is tested on Mac and Linux, but not tested on Windows.

`npm install -s xlsx-util`

Basic usage

```
const xlsx = require('xlsx-util')
const data = xlsx.readFile('myfilepath.xlsx')
```

Given an xlsx file like this

```
     A       B     C
   -----------------------------
1  | Hello |     | World       |
   -----------------------------
2  | 1     | 2   | 4           |
   -----------------------------
3  |       |     |             |
   -----------------------------
4  | 5     | 6   |             |
   -----------------------------
5  |       |     |             |
   -----------------------------
6  | pp    | or  | 0.085714286 |
   -----------------------------
```

We will get an array of objects like this:


