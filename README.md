# cs50-filter-more-implementation
Filter-More (Comfortable) implementation Description

# Generalized Edges Filter (Sobel Operator Implementation)
# 泛化边缘检测滤镜 (Sobel 算子实现)

## Project Overview (项目简介)
This project is an advanced implementation of the **Edge Detection** filter for CS50's "Filter-More" problem set.
Instead of hard-coding the standard 3x3 Sobel kernel, I developed a **generalized algorithm** based on coordinate geometry. This approach allows the filter to dynamically calculate weights using **Manhattan Distance**, making it possible to adjust the kernel radius (e.g., from 3x3 to 5x5 or larger) programmatically.

本项目是哈佛 CS50 课程 "Filter-More" 作业中 **边缘检测 (Edges)** 功能的高级实现。
我没有使用硬编码的标准 3x3 Sobel 核，而是开发了一套基于坐标几何的 **泛化算法**。该算法利用 **曼哈顿距离 (Manhattan Distance)** 动态计算权重，从而在程序逻辑上支持了可调节的滤镜半径（例如从 3x3 扩展到 5x5 或更大）。

## Key Features (核心特色)

### 1. Dynamic Kernel Generation (动态核生成)
Rather than using a fixed 2D array, I derived a formula to calculate the Sobel weights on the fly based on the pixel's distance from the center.
我没有使用固定的二维数组，而是推导出了一个公式，根据像素距离中心的偏移量实时计算 Sobel 权重。

**The Logic (核心逻辑):**
```c
// Based on Manhattan Distance (Linear Decay)
// 基于曼哈顿距离的线性衰减
int dist_x = abs(column - center_j);
int dist_y = abs(row - center_i);
int weight = radius - (dist_x + dist_y);

2. Full Boundary Handling (全边界处理)
Implemented a robust system to handle image edges and corners without segmentation faults, ensuring every pixel is processed correctly regardless of the kernel size. 实现了一套健壮的边界处理系统，无论卷积核大小如何，都能确保图像边缘和角落的像素被正确处理，杜绝内存越界错误。

Dev Log: The Intersection of Art and Code
From Photoshop to C: Re-imagining Image Processing (从 PS 到 C：重构图像处理的认知)
edges的代码依旧是保持了可调节半径的想法，坚持用硬逻辑写完了soble取值。这1段处理图像的知识与我自己的设计经验交叉结合，算是用代码视角重新了解了1遍计算机是如何达成软件里的操作效果，利用视觉知识积累的直觉和ai讨论了不少映射到计算机或数学的同领域知识。 ai用absolute value函数简化了1下我用if手写的取值代码，补充了1点数学知识。感觉这种交叉到自己熟悉领域的部分是很建立自信心的1课。

