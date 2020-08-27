#include "VertexLayout.h"

void VertexLayout::use()
{
	//׼��vertexAttributePointer
	//����ÿ��vertexAttribute��Ϊ�˷��������opengl��Ҫ֪��
	//1.�������ڱ����ĸ�����(index)
	//2.������Դ��Ŀ�ʼ���ҵ�һ��ȥ����(pointer)
	//3.������Զ��һ���Զ��೤���ڴ�(size)
	//4.������Ա�����ʲô��������ڴ浱��ʲô����(type)
	//5.����������ڴ������������󣬷�����ֱ��������һ���ڴ��ȡ(stride)
	//6.��������Ƿ񱻹�һ������Ҫ��Ҫֱ����������ݻ����Լ�����һ��
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
