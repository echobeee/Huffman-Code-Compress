### 这是最简单的huffman编码压缩
实现的功能：
1. huffman编码压缩
2. 解压缩（无损）
### 缺陷：
1. 效率extremely低下，对于较小的文件还可以，一旦文件大小变大，压缩时间相当长！
2. 目前只能压缩[英文][文本]文件(txt、pdf、docx)，只有此类文件能成功解压缩回去。
3. 对于其他的如jpg、bmp、含有中文字体的pdf、docx、txt等其他格式文件，都无法很好的解压缩，或是解压缩后的文件无法打开，或是成为乱码。
