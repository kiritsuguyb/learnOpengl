#include "VertexLayout.h"

void VertexLayout::use()
{
	//准备vertexAttributePointer
	//对于每个vertexAttribute，为了方便遍历，opengl需要知道
	//1.现在是在遍历哪个属性(index)
	//2.这个属性从哪开始，我第一个去哪里(pointer)
	//3.这个属性多大，一次性读多长的内存(size)
	//4.这个属性本质是什么，把这段内存当作什么看待(type)
	//5.这个属性在内存里两两间隔多大，方便我直接跳到下一个内存读取(stride)
	//6.这个属性是否被归一化，我要不要直接用这个数据还是自己处理一下
	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/
	unsigned int offset = 0;
	for (int i = 0; i < layout.size(); i++) {
		glVertexAttribPointer(i,layout[i].count,layout[i].type,GL_FALSE,stride, (void*)offset);
		glEnableVertexAttribArray(i);
		offset += layout[i].size*layout[i].count;
	}
}
