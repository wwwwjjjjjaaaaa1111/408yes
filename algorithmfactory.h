#ifndef ALGORITHMFACTORY_H
#define ALGORITHMFACTORY_H

#include "algorithmbase.h"
#include <QVector>
#include <memory>

// Chapter 3
#include "bracketmatching.h"
#include "expressioneval.h"
#include "recursionstack.h"
#include "levelorder.h"
#include "bufferqueue.h"

// Chapter 4
#include "bruteforce.h"
#include "kmp.h"
#include "kmpnextval.h"

// Chapter 5
#include "binarytreetraversal.h"
#include "threadedtree.h"
#include "treetransform.h"
#include "foresttraversal.h"
#include "huffman.h"
#include "unionfind.h"

// Chapter 6
#include "bfs.h"
#include "dfs.h"
#include "mst.h"
#include "shortestpath.h"
#include "toposort.h"
#include "criticalpath.h"

// Chapter 7
#include "sequentialsearch.h"
#include "binarysearch.h"
#include "blocksearch.h"
#include "bst.h"
#include "avl.h"
#include "rbtree.h"
#include "btree.h"
#include "bplustree.h"
#include "hashtable.h"

// Chapter 8
#include "insertionsort.h"
#include "shellsort.h"
#include "bubblesort.h"
#include "quicksort.h"
#include "selectionsort.h"
#include "heapsort.h"
#include "mergesort.h"
#include "radixsort.h"
#include "countingsort.h"
#include "externalsort.h"

class AlgorithmFactory {
public:
    struct Category {
        QString name;
        QVector<QString> algorithmNames;
    };

    static QVector<Category> categories() {
        return {
            {"第3章 栈、队列和数组", {
                "栈在括号匹配中的应用",
                "栈在表达式求值中的应用",
                "栈在递归中的应用",
                "队列在层次遍历中的应用",
                "队列在计算机系统中的应用"
            }},
            {"第4章 串", {
                "简单的模式匹配",
                "KMP算法",
                "KMP算法的优化"
            }},
            {"第5章 树与二叉树", {
                "二叉树的四种遍历",
                "线索二叉树",
                "树、森林与二叉树的转换",
                "树和森林的遍历",
                "哈夫曼树和哈夫曼编码",
                "并查集"
            }},
            {"第6章 图", {
                "广度优先搜索(BFS)",
                "深度优先搜索(DFS)",
                "最小生成树(Prim & Kruskal)",
                "最短路径(Dijkstra & Floyd)",
                "拓扑排序",
                "关键路径"
            }},
            {"第7章 查找", {
                "顺序查找",
                "折半查找",
                "分块查找",
                "二叉排序树(BST)",
                "平衡二叉树(AVL)",
                "红黑树",
                "B树",
                "B+树",
                "散列表(Hash)"
            }},
            {"第8章 排序", {
                "插入排序(直接/折半/希尔)",
                "交换排序(冒泡/快速)",
                "选择排序(简单选择/堆排序)",
                "归并排序",
                "基数排序",
                "计数排序",
                "外部排序"
            }}
        };
    }

    static AlgorithmBase *create(const QString &name, QObject *parent = nullptr) {
        if (name == "栈在括号匹配中的应用") return new BracketMatching(parent);
        if (name == "栈在表达式求值中的应用") return new ExpressionEval(parent);
        if (name == "栈在递归中的应用") return new RecursionStack(parent);
        if (name == "队列在层次遍历中的应用") return new LevelOrder(parent);
        if (name == "队列在计算机系统中的应用") return new BufferQueue(parent);

        if (name == "简单的模式匹配") return new BruteForceMatch(parent);
        if (name == "KMP算法") return new KMPMatch(parent);
        if (name == "KMP算法的优化") return new KMPNextVal(parent);

        if (name == "二叉树的四种遍历") return new BinaryTreeTraversal(parent);
        if (name == "线索二叉树") return new ThreadedTree(parent);
        if (name == "树、森林与二叉树的转换") return new TreeTransform(parent);
        if (name == "树和森林的遍历") return new ForestTraversal(parent);
        if (name == "哈夫曼树和哈夫曼编码") return new HuffmanCoding(parent);
        if (name == "并查集") return new UnionFind(parent);

        if (name == "广度优先搜索(BFS)") return new BFS(parent);
        if (name == "深度优先搜索(DFS)") return new DFS(parent);
        if (name == "最小生成树(Prim & Kruskal)") return new MST(parent);
        if (name == "最短路径(Dijkstra & Floyd)") return new ShortestPath(parent);
        if (name == "拓扑排序") return new TopoSort(parent);
        if (name == "关键路径") return new CriticalPath(parent);

        if (name == "顺序查找") return new SequentialSearch(parent);
        if (name == "折半查找") return new BinarySearch(parent);
        if (name == "分块查找") return new BlockSearch(parent);
        if (name == "二叉排序树(BST)") return new BST(parent);
        if (name == "平衡二叉树(AVL)") return new AVLTree(parent);
        if (name == "红黑树") return new RBTree(parent);
        if (name == "B树") return new BTree(parent);
        if (name == "B+树") return new BPlusTree(parent);
        if (name == "散列表(Hash)") return new HashTable(parent);

        if (name == "插入排序(直接/折半/希尔)") return new InsertionSort(parent);
        if (name == "交换排序(冒泡/快速)") return new ExchangeSort(parent);
        if (name == "选择排序(简单选择/堆排序)") return new SelectionSort(parent);
        if (name == "归并排序") return new MergeSort(parent);
        if (name == "基数排序") return new RadixSort(parent);
        if (name == "计数排序") return new CountingSort(parent);
        if (name == "外部排序") return new ExternalSort(parent);

        return nullptr;
    }
};

#endif // ALGORITHMFACTORY_H
