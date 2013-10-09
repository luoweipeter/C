#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define FALSE 0
#define TRUE 1
#define OK 1
#define ERROR 0
#define MAX_VERTEX_NUM 20
#define MAX_NAME 20
#define STACK_INIT_SIZE 10 // 存储空间初始分配量
#define STACKINCREMENT 2 // 存储空间分配增量
#define STACK_INIT_SIZE 10  //定义最初申请的内存的大小
#define STACK_INCREMENT 2  //每一次申请内存不足的时候扩展的大小
#define OVERFLOW false  //异常抛出返回值

typedef int TElemType;
typedef int InfoType;
typedef int VertexType;
typedef bool Boolean;
typedef bool Status;
typedef int SElemType;


/******************************** 数据结构区 ********************************/

typedef struct CSNode //孩子兄弟树结点类型
{
	TElemType data;
	CSNode *firstchild, *nextsibling;
} CSNode, *CSTree; //CSTree是孩子兄弟树类型


struct ArcNode
{
	int adjvex; // 该弧所指向的顶点的位置
	ArcNode *nextarc; // 指向下一条弧的指针
	InfoType *info; // 网的权值指针
}; // 表结点

typedef struct VNode
{
	VertexType data; // 顶点信息
	ArcNode *firstarc; // 第一个表结点的地址,指向第一条依附该顶点的弧的指针
} VNode, AdjList[MAX_VERTEX_NUM]; // 头结点

typedef struct
{
	AdjList vertices; //以头结点表示邻接表
	int vexnum, arcnum; // 图的当前顶点数和弧数
	int kind; // 图的种类标志
} ALGraph;

struct Stack
{
//栈的数据元素类型在应用程序中定义typedef int SElemType;
	SElemType *base; // 栈底指针
//在栈构造之前和销毁之后，base为NULL
	SElemType *top; // 栈顶指针
//初值指向栈底，top = base可做栈空标记
//插入新栈顶元素时，top增1；删除栈顶元素时，top减1
//非空栈的top始终在栈顶元素的下一个位置上
	int stacksize; // 当前已分配的存储空间，以元素为单位
//栈当前可使用的最大容量
}; // 顺序栈





/******************************** 基本函数 ********************************/
Boolean visited[MAX_VERTEX_NUM];

VertexType& GetVex(ALGraph G, int v)
{
	// 初始条件: 无向图G存在,v是G中某个顶点的序号。操作结果: 返回v的值
	if (v >= G.vexnum || v < 0)
		exit(ERROR);

	return G.vertices[v].data;
}

int LocateVex(ALGraph G, VertexType u)
{
	// 初始条件: 图G存在,u和G中顶点有相同特征
// 操作结果: 若G中存在顶点u,则返回该顶点在图中位置;否则返回-1
	int i;

	for (i=0; i<G.vexnum; ++i) //依次扫描每个头结点的顶点信息
		if (u== G.vertices[i].data) //如果当前头结点的顶点信息等于u
			return i; //返回当前头结点的序号

	return -1;
}

Status CreateGraph(ALGraph &G)
{
	// 采用邻接表存储结构,构造没有相关信息的图G(用一个函数构造4种图)
	int i, j, k;
	int w; // 权值
	VertexType va, vb; //顶点va、vb
	ArcNode *p; //表结点指针p

	printf("请输入图的类型(有向图:0,有向网:1,无向图:2,无向网:3):(这里以无向网为例)\n ");
	scanf("%d", &G.kind);

	printf("请输入图的顶点数 边数: ");
	scanf("%d%d", &G.vexnum, &G.arcnum);

//MAX_NAME，顶点字符串的最大长度+1
	printf("请输入%d个顶点的值(<%d个字符):\n", G.vexnum, MAX_NAME);

	for (i=0; i<G.vexnum; ++i) // 构造顶点向量，构造各个头结点
	{
		scanf("%d",&G.vertices[i].data);  //为各个头结点输入名字
		G.vertices[i].firstarc = NULL;  //链域置空
	}

	if (G.kind == 1 || G.kind == 3) // 网
		printf("请顺序输入每条弧(边)的权值、弧尾和弧头(以空格作为间隔):\n");
	else // 图
		printf("请顺序输入每条弧(边)的弧尾和弧头(以空格作为间隔):\n");

	for (k=0; k<G.arcnum; ++k) // 构造表结点链表
	{
		if (G.kind == 1 || G.kind == 3) // 网
			scanf("%d%d%d", &w,&va,&vb); //接收每条弧(边)的权值、弧尾和弧头
		else // 图
			scanf("%d%d",&va,&vb); //接收每条弧(边)的弧尾和弧头

		i = LocateVex(G, va); // i取弧尾序号
		j = LocateVex(G, vb); // j取弧头序号

		//p指向新分配的表结点空间
		p = (ArcNode*) malloc(sizeof (ArcNode));
		p->adjvex = j; //该弧所指向的顶点的位置，为什么是弧头？为求入度，假如是弧尾，则是求出度
		if (G.kind == 1 || G.kind == 3) // 网
		{
			p->info = (int *) malloc(sizeof (int)); //分配网的权值的空间
			*(p->info) = w; //网的权值取w
		}
		else
			p->info = NULL; // 图

		//新弧p的下一条弧取当前头结点的第一条弧
		p->nextarc = G.vertices[i].firstarc; // 插在表头
		G.vertices[i].firstarc = p; //当前头结点的第一条弧取新弧p

		if (G.kind >= 2) // 无向图或网,产生第二个表结点
		{
			p = (ArcNode*) malloc(sizeof (ArcNode)); //分配表结点
			p->adjvex = i; //该弧所指向的顶点的位置，取弧尾

			if (G.kind == 3) // 无向网
			{
				p->info = (int*) malloc(sizeof (int)); //分配网的权值的空间
				*(p->info) = w; //网的权值取w
			}
			else
				p->info = NULL; // 无向图

			//新弧p的下一条弧取当前头结点的第一条弧
			p->nextarc = G.vertices[j].firstarc; // 插在表头
			G.vertices[j].firstarc = p; //当前头结点的第一条弧取新弧p
		}
	}
	return OK;
}
int FirstAdjVex(ALGraph G, VertexType v)
{
	// 初始条件: 图G存在,v是G中某个顶点
// 操作结果: 返回v的第一个邻接顶点的序号。若顶点在G中没有邻接顶点,则返回-1
	ArcNode *p;
	int v1;

	v1 = LocateVex(G, v); // v1取顶点v的序号
	p = G.vertices[v1].firstarc; //p指向顶点v的第一条弧

	if (p)
		return p->adjvex; //返回顶点v的第一条弧所指向顶点的序号
	else
		return -1;
}
int NextAdjVex(ALGraph G, VertexType v, VertexType w)
{
	// 初始条件: 图G存在,v是G中某个顶点,w是v的邻接顶点
// 操作结果: 返回v的(相对于w的)下一个邻接顶点的序号。
//           若w是v的最后一个邻接点,则返回-1
	ArcNode *p;
	int v1, w1;

	v1 = LocateVex(G, v); // v1取顶点v的序号
	w1 = LocateVex(G, w); // w1取顶点w的序号

	p = G.vertices[v1].firstarc; //p指向顶点v的第一条弧

	while (p && p->adjvex != w1) // 顶点v的第一条弧存在、且顶点v的第一条弧所指顶点不是w
		p = p->nextarc; //p后移，依次指向顶点v的第二、第三、第...条弧
//循环结束时，顶点v的第...条弧不存在、或者顶点v的第...条弧所指顶点是w

	if (!p || !p->nextarc) // 没找到w、或w是最后一个邻接点
		//p->nextarc为NULL说明w是最后一个邻接点
		return -1;
	else // p->adjvex == w
		return p->nextarc->adjvex; // 返回v的(相对于w的)下一个邻接顶点的序号
}

void DFSTree(ALGraph G, int v, CSTree &T)
{
	// 从第v个顶点出发深度优先遍历图G,建立以T为根的生成树。算法7.8
	Boolean first = TRUE;
	int w;
	CSTree p, q; //孩子兄弟树p、q
	VertexType v1, w1;

	visited[v] = TRUE;
	v1=GetVex(G, v); //顶点v1序号为v

// w依次为v的邻接顶点
//w初始化为v1第一个邻接点序号
//只要w大于等于0，就执行循环体
//循环体完毕，w取v1的(相对于w1（序号w）的)下一个邻接顶点的序号，准备下轮循环
	for (w=FirstAdjVex(G,v1); w>=0; w=NextAdjVex(G,v1,w1=GetVex(G,w)))
		if (!visited[w]) // w顶点不曾被访问
		{
			p = (CSTree) malloc(sizeof (CSNode)); // 分配新孩子结点

			p->data=GetVex(G, w); //v的当前邻接顶点复制到新结点
			p->firstchild = NULL;
			p->nextsibling = NULL;

			if (first) // w是v的第一个未被访问的邻接顶点
			{
				T->firstchild = p; //树T的长子取新结点
				first = FALSE; // 长子标志置假
			}
			else // w是v的其它未被访问的邻接顶点，是上一邻接顶点的兄弟结点
				q->nextsibling = p; // 上一邻接顶点的兄弟结点取新结点

			q = p; //q取新结点
			DFSTree(G, w, q); // 从第w个顶点出发深度优先遍历图G,递归建立以q为根的子生成树
		}
}

void DFSForest(ALGraph G, CSTree &T)
{
	// 建立无向图G的深度优先生成森林的(最左)孩子(右)兄弟链表T。算法7.7
	CSTree p, q; //孩子兄弟树p、q
	int v;

	T = NULL;

	for (v=0; v<G.vexnum; ++v)
		visited[v] = FALSE; // 赋初值

	for (v=0; v<G.vexnum; ++v) // 从第0个顶点找起
		if (!visited[v]) // 第v个顶点不曾被访问
		{
			// 第v顶点为新的生成树的根结点
			p = (CSTree) malloc(sizeof (CSNode)); // 分配根结点

			p->data=GetVex(G, v); //当前顶点（第v个）拷贝到新分配的根结点
			p->firstchild = NULL;
			p->nextsibling = NULL;

			if (!T) // 如果孩子兄弟树T为空，表示是第一棵生成树的根(T的根)
				T = p; //所以T取新分配的根结点
			else // 如果孩子兄弟树T不为空，表示是其它生成树的根(前一棵的根的“兄弟”)
				q->nextsibling = p; //令前一棵的根的“兄弟”取新分配的根结点

			q = p; // q取当前生成树的根
			DFSTree(G, v, p); // 从第v顶点出发建立以p为根的生成树
		}
}
Status OutPutALGraph(ALGraph G)
{
//以无向网为例
	int i;
	for(i=0; i<G.vexnum; i++)
	{
		printf("%s\t",G.vertices[i].data);
		while(G.vertices[i].firstarc)
		{
			printf("%d\t",G.vertices[i].firstarc->adjvex);
			G.vertices[i].firstarc=G.vertices[i].firstarc->nextarc;
		}
		printf("\n");
	}



	return 0;
}
Status InitTree(CSTree &T)
{
	// 操作结果: 构造空树T
	T = NULL;
	return OK;
}

/********************** 构造一个空栈S **********************/
Status InitStack(Stack &S)
{
	// 构造一个空栈S
//栈底指针S.base指向新分配的STACK_INIT_SIZE个SElemType大小的空间
	if (!(S.base = (SElemType *) malloc(STACK_INIT_SIZE * sizeof (SElemType))))
		exit(OVERFLOW); // 存储分配失败

//必须给SqStack结构对象S的3个成员赋值
	S.top = S.base; //空栈的栈顶指针S.top = 栈底指针S.base
	S.stacksize = STACK_INIT_SIZE; //初始栈容量
	return OK;
}
/********************** 插入新的栈顶元素 **********************/
Status Push(Stack &S, SElemType e)
{
	// 插入元素e为新的栈顶元素
// if (S.top - S.base >= S.stacksize) // 栈满，追加存储空间
	//如果栈长度大于栈容量
// {
	//可能会改变栈底地址，新栈底指针S.base
//  S.base = (SElemType *) realloc(S.base, //原栈底指针
//   (S.stacksize + STACKINCREMENT) * sizeof (SElemType)); //新大小

//  if (!S.base)
//   exit(OVERFLOW); // 存储分配失败
//
	//给SqStack结构对象S的3个成员赋值
//  S.top = S.base + S.stacksize;
//  S.stacksize += STACKINCREMENT;
// }
	*(S.top)++ = e; //先把e压入栈顶，S.top再增1指向栈顶元素e的下一个位置
	return OK;
}

/********************** 删除栈顶元素 **********************/
Status Pop(Stack &S)
{
	// 若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR
	int e;
	if (S.top == S.base) //如果空栈，报错
		return ERROR;

	e = *--S.top; //S.top先减1指向栈顶元素，再取值，交给e带回
	return OK;
}

/********************** 判断栈是否是空的 **********************/
Status output(int e)
{
	printf("%d",e);
	return 0;
}
Status StackTraverse(Stack S, Status (*visit) (SElemType))
{
	// 从栈底到栈顶依次对栈中每个元素调用函数visit()。
// 一旦visit()失败，则操作失败
	while (S.top > S.base) //从栈底元素开始，到栈顶元素为止
		visit(*S.base++); //依次取栈底指针指向元素调用visit()，栈底指针后（上）移
//循环结束时，S.base = S.top

	printf("\n");
	return OK;
}
void OutPath(CSTree T)
{
	// 依次从左到右输出森林中每一棵树中从根到叶的路径
// 森林的存储结构为孩子-兄弟链表，T为头指针
	Stack S; //栈S存储路径
	InitStack(S);
	if (T)
	{
		Push(S,T->data); // 根结点加入路径S

		if (!T->firstchild)
			StackTraverse(S, output); // 求得一条路径
		//从栈底到栈顶依次对栈S中每个元素调用函数output()
		else
			OutPath(T->firstchild); // 递归，遍历子树森林

		Pop(S); // 从路径中删除栈顶结点

		OutPath(T->nextsibling); // 递归，遍历其余树的森林
	}
}


/******************************** 主函数 ********************************/

int main()
{
	ALGraph G;
	CSTree T;
	CreateGraph(G);
	InitTree(T);
	DFSForest(G,T);
	OutPath(T);


	return 0;
}