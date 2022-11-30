#include <iostream>
#include <cmath>
#include<typeinfo>
#include<set>
using namespace std;
template <class V, class T>
class TreeIterator;

enum type
{
    asteroid,
    natural_satellite,
    planet,
    star,
    quasar,
};

class HeavenlBody
{
private:
    type object;
    string name;
    int number;
    int range;
    int mass = 0;
    string mass1 = "none";
public:
    HeavenlBody()
    {
        object = asteroid;
        number = 0;
        range = 999;
        mass = 1000;
        name = "name";
    }
    HeavenlBody(type object1, int num, int ran, int ma, string n = "none")// конструктор в который подается масса числовая
    {
        object = object1;
        number = num;
        range = ran;
        mass = ma;
        name = n;
    }
    HeavenlBody(type object1, int num, int ran, string ma, string n = "none")// конструктор в который подается масса инф
    {
        object = object1;
        number = num;
        range = ran;
        mass1 = ma;
        name = n;
    }

    int getKey()
    {
        return number;
    }

    bool operator> (HeavenlBody ob)
    {
        return number > ob.number;
    }
    bool operator<(HeavenlBody ob)
    {
        return number < ob.number;
    }

    bool operator==(HeavenlBody ob)
    {
        return number == ob.number;
    }
    bool operator!=(HeavenlBody ob)
    {
        return !(number == ob.number);
    }
    friend ostream& operator<<(ostream& s, HeavenlBody a);
    friend bool operator<(const HeavenlBody& ot1, const HeavenlBody& ot2);
};

ostream& operator<<(ostream& s, HeavenlBody a)
{
    s << "_________________________" << endl;
    s << "Имя объекта: " << a.name << endl;
    s << "Тип объекта: " << a.object << endl;
    s << "Число в списке: " << a.number << "	<------- " << endl;
    s << "Удаление от земли: " << a.range << endl;
    if (a.mass1 == "Inf")
        s << "Масса объекта: " << a.mass1 << endl;
    else
        s << "Масса объекта: " << a.mass << endl;

    return s;
}

bool operator<(const HeavenlBody& ot1, const HeavenlBody& ot2)
{
    return ot1.number < ot2.number;
}

bool biger(double value)
{
    double a = 3.7873897;
    return(abs(value) > a);
}


template <class V, class T>
class Node
{
protected:
    T data;
    V key;

    //не можем хранить Node, но имеем право хранить указатель
    Node* left;
    Node* right;
    Node* parent;

    //переменная, необходимая для поддержания баланса дерева
    int height;

public:
    //доступные извне переменные и функции
    virtual void setData(T d) { data = d; }
    virtual T getData() { return data; }
    int getHeight() { return height; }
    virtual V getKey() { return key; }
    V getkey()
    {
        return data.getKey();
    }

    virtual Node* getLeft() { return left; }
    virtual Node* getRight() { return right; }
    virtual Node* getParent() { return parent; }

    virtual void setLeft(Node* N) { left = N; }
    virtual void setRight(Node* N) { right = N; }
    virtual void setParent(Node* N) { parent = N; }

    //Конструктор. Устанавливаем стартовые значения для указателей
    Node<V, T>(T n)
    {
        data = n;
        left = right = parent = NULL;
        height = 1;
    }

    Node<V, T>()
    {
        left = NULL;
        right = NULL;
        parent = NULL;
        height = 1;
    }
    Node<V, T> operator=(const Node<V, T> a)
    {
        left = a.left;
        right = a.right;
        parent = a.parent;
        height = a.height;
        data = a.data;
        key = a.key;
        return *this;
    }



    virtual void print()
    {
        cout << "\n"
            << data;
    }

    virtual void setHeight(int h)
    {
        height = h;
    }

    friend ostream& operator<<(ostream& stream, Node<V, T>& N)
    {
        stream << "\nNode data: " << N.data << ", height: " << N.height;
        return stream;
    }
};

template <class V, class T>
void print(Node<V, T>* N) {
    cout << "\n"
        << N->getData();
}

template <class V, class T>
class Tree
{
protected:
    //корень - его достаточно для хранения всего дерева
    Node<V, T>* root;

    virtual void clear(Node<V, T>* p)
    {
        if (p != nullptr)
        {
            clear(p->getLeft());
            clear(p->getRight());

            delete p;
            p = nullptr;
        }
    }

public:
    virtual ~Tree()
    {
        clear(Tree<V, T>::root);
    }
    //доступ к корневому элементу
    virtual Node<V, T>* getRoot() { return root; }

    //конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
    Tree<V, T>() { root = NULL; }

    //рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
    virtual Node<V, T>* push_R(Node<V, T>* N)
    {
        return push_R(N, root);
    }

    virtual Node<V, T>* push_R(Node<V, T>* N, Node<V, T>* Current)
    {
        if (N == NULL)
            return NULL;
        if (root == NULL)
        {
            root = N;
            return N;
        }

        if (Current->getData() > N->getData())
        {
            //идем влево
            if (Current->getLeft() != NULL)
                Current->setLeft(push_R(N, Current->getLeft()));
            else
                Current->setLeft(N);
            Current->getLeft()->setParent(Current);
        }
        if (Current->getData() < N->getData())
        {
            //идем вправо
            if (Current->getRight() != NULL)
                Current->setRight(push_R(N, Current->getRight()));
            else
                Current->setRight(N);
            Current->getRight()->setParent(Current);
        }
        if (Current->getData() == N->getData())
            //нашли совпадение
            ;
        //для несбалансированного дерева поиска
        return Current;
    }

    //функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
    virtual Node<V, T>* push(T n)
    {
        Node<V, T>* N = new Node<V, T>;
        N->setData(n);
        return push_R(N);
    }

    //удаление узла
    Node<V, T>* Remove(Node<V, T>* N)
    {
        if (N == NULL)
            return NULL;
        if (root == NULL)
            return NULL;

        TreeIterator<V, T> it = Min();
        while (it != NULL)
        {
            if (it == N) break;
            it++;
        }
        if (*it == NULL)
            return NULL;

        Node<V, T>* node = *it;
        Node<V, T>* parent = nullptr;
        Node<V, T>* left = nullptr;
        Node<V, T>* right = nullptr;
        Node<V, T>* rightleft = nullptr;

        if (node->getParent() != NULL)
            parent = node->getParent();
        if (node->getLeft() != NULL)
            left = node->getLeft();
        if (node->getRight() != nullptr)
            right = node->getRight();
        if (node->getRight() != nullptr)
        {
            if (right->getLeft() != nullptr)
                rightleft = right->getLeft();
        }
        if (parent->getRight() == node) // 
        {
            if (parent != nullptr)
                parent->setRight(right);
            if (right != nullptr)
                right->setParent(parent);
            if (left != nullptr)
                left->setParent(right);
            if (right != nullptr)
                right->setLeft(left);
            if (rightleft != nullptr)
                this->push(rightleft->getData());
        }
        else if (parent->getLeft() == node)
        {
            if (parent != nullptr)
                parent->setLeft(right);
            if (right != nullptr)
                right->setParent(parent);
            if (left != nullptr)
                left->setParent(right);
            if (right != nullptr)
                right->setLeft(left);
            if (rightleft != nullptr)
                this->push(rightleft->getData());
        }
        return *it;

    }
    Node <V, T>* RemoveByKey(V key)
    {
        if (root == NULL)
            return nullptr;

        TreeIterator<V, T> it = Min();
        while (it != nullptr)
        {
            if ((*it)->getkey() == key) break;
            it++;
        }
        if (*it == nullptr)
            return nullptr;
        Remove(*it);
        return *it;
    }
    Node <V, T>* RemoveByValue(T value)
    {
        if (root == NULL)
            return nullptr;

        TreeIterator<V, T> it = Min();
        while (it != nullptr)
        {
            if ((*it)->getData() == value) break;
            it++;
        }
        if (*it == nullptr)
            return nullptr;
        Remove(*it);
        return *it;
    }
    virtual Node<V, T>* Min(Node<V, T>* Current = NULL)
    {
        //минимум - это самый "левый" узел. Идём по дереву всегда влево
        if (root == NULL)
            return NULL;

        if (Current == NULL)
            Current = root;
        while (Current->getLeft() != NULL)
            Current = Current->getLeft();

        return Current;
    }

    virtual Node<V, T>* Max(Node<V, T>* Current = NULL)
    {
        //минимум - это самый "правый" узел. Идём по дереву всегда вправо
        if (root == NULL)
            return NULL;

        if (Current == NULL)
            Current = root;
        while (Current->getRight() != NULL)
            Current = Current->getRight();

        return Current;
    }

    //поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
    virtual Node<V, T>* Find(T data, Node<V, T>* Current)
    {

        return  Current == nullptr ? nullptr :
            Current->getData() == data ? Current :
            Current->getData() > data ? Find(data, Current->getLeft()) :
            Current->getData() < data ? Find(data, Current->getRight()) : nullptr;
    }

    //три обхода дерева
    virtual void PreOrder(Node<V, T>* N, void (*f)(Node<V, T>*))
    {
        if (N != NULL)
            f(N);
        if (N != NULL && N->getLeft() != NULL)
            PreOrder(N->getLeft(), f);
        if (N != NULL && N->getRight() != NULL)
            PreOrder(N->getRight(), f);
    }

    //InOrder-обход даст отсортированную последовательность
    virtual void InOrder(Node<V, T>* N, void (*f)(Node<V, T>*))
    {
        if (N != NULL && N->getLeft() != NULL)
            InOrder(N->getLeft(), f);
        if (N != NULL)
            f(N);
        if (N != NULL && N->getRight() != NULL)
            InOrder(N->getRight(), f);
    }

    virtual void PostOrder(Node<V, T>* N, void (*f)(Node<V, T>*))
    {
        if (N != NULL && N->getLeft() != NULL)
            PostOrder(N->getLeft(), f);
        if (N != NULL && N->getRight() != NULL)
            PostOrder(N->getRight(), f);
        if (N != NULL)
            f(N);
    }
};


template <class V, class T>
class IteratedTree : public Tree<V, T>
{
public:
    IteratedTree<V, T>() : Tree<V, T>() {}


    TreeIterator<V, T> begin()
    {
        TreeIterator<V, T> it = Tree<V, T>::Min();
        return it;
    }
    TreeIterator<V, T> end()
    {
        TreeIterator<V, T> it = Tree<V, T>::Max();
        return it;
    }
};

template <class V, class T>
class TreeIterator : public std::iterator<std::input_iterator_tag, T>
{
protected:
    Node<V, T>* node;

    Node<V, T>* successor_in_order(Node<V, T>* t)
    {
        Node<V, T>* temp;

        if (t->getRight() != NULL)
        {
            temp = t->getRight();
            while (temp->getLeft() != NULL)
            {
                temp = temp->getLeft();
            }
            return temp;
        }

        if (t->getParent()->getLeft() == t)
        {
            temp = t->getParent();
            return temp;
        }

        temp = t->getParent();
        while (temp->getParent() && temp->getParent()->getLeft() != temp)
        {
            temp = temp->getParent();
        }

        temp = temp->getParent();
        return temp;
    }

    Node<V, T>* predecessor_in_oreder(Node<V, T>* t)
    {
        Node<V, T>* temp;
        if (t->getLeft() != NULL)
        {
            temp = t->getLeft();

            while (temp->getRight() != NULL)
            {
                temp = temp->getRight();
            }
            return temp;
        }

        if (t->getParent()->getRight() == t)
        {

            temp = t->getParent();
            return temp;
        }

        temp = t->getParent();
        while (temp->getParent() && temp->getParent()->getRight() != temp)
        {
            temp = temp->getParent();
        }

        temp = temp->getParent();
        return temp;
    }

public:
    TreeIterator() { node = NULL; }
    TreeIterator(Node<V, T>* n)
    {
        node = n;
    }
    TreeIterator(const TreeIterator<V, T>& other)
    {
        node = other.node;
    }


    Node<V, T>* operator*()
    {
        if (!node)
            throw 1;
        else
            return node;
    }

    TreeIterator operator++(int)
    {
        node = successor_in_order(node);
        return TreeIterator(node);
    }

    TreeIterator operator--(int)
    {
        node = predecessor_in_oreder(node);
        return TreeIterator(node);
    }

    bool operator==(Node<V, T>* rhs)
    {
        return node == rhs;
    }
    bool operator==(TreeIterator<V, T> rhs)
    {
        return node == rhs.node;
    }

    bool operator!=(Node<V, T>* rhs)
    {
        return !(node == rhs);
    }


};

template <class V, class T>
class AVL_Tree : public IteratedTree<V, T>
{
protected:
    //определение разности высот двух поддеревьев
    int bfactor(Node<V, T>* p)
    {
        int hl = 0;
        int hr = 0;
        if (p->getLeft() != NULL)
            hl = p->getLeft()->getHeight();
        if (p->getRight() != NULL)
            hr = p->getRight()->getHeight();
        return (hr - hl);
    }

    //при добавлении узлов в них нет информации о балансе, т.к. не ясно, куда в дереве они попадут
    //после добавления узла рассчитываем его высоту (расстояние до корня) и редактируем высоты в узлах, где это
    //значение могло поменяться
    void fixHeight(Node<V, T>* p)
    {
        int hl = 0;
        int hr = 0;
        if (p->getLeft() != NULL)
            hl = p->getLeft()->getHeight();
        if (p->getRight() != NULL)
            hr = p->getRight()->getHeight();
        p->setHeight((hl > hr ? hl : hr) + 1);
    }

    //краеугольные камни АВЛ-деревьев - процедуры поворотов
    Node<V, T>* RotateRight(Node<V, T>* p) // правый поворот вокруг p
    {
        Node<V, T>* q = p->getLeft();
        if (p->getParent() != nullptr)
        {
            if (p->getParent()->getLeft() == p)
                p->getParent()->setLeft(q);
            else
                p->getParent()->setRight(q);
        }
        q->setParent(p->getParent());
        p->setParent(q);
        if (p == Tree<V, T>::root)
            Tree<V, T>::root = q;

        p->setLeft(q->getRight());
        q->setRight(p);

        fixHeight(p);
        fixHeight(q);
        return q;
    }

    Node<V, T>* RotateLeft(Node<V, T>* q) // левый поворот вокруг q
    {
        Node<V, T>* p = q->getRight();

        if (q->getParent() != nullptr)
        {
            if (q->getParent()->getLeft() == q)
                q->getParent()->setLeft(p);
            else
                q->getParent()->setRight(p);
        }

        p->setParent(q->getParent());
        q->setParent(p);
        if (q == Tree<V, T>::root)
            Tree<V, T>::root = p;

        q->setRight(p->getLeft());
        p->setLeft(q);

        fixHeight(q);
        fixHeight(p);
        return p;
    }

    //балансировка поддерева узла p - вызов нужных поворотов в зависимости от показателя баланса
    Node<V, T>* Balance(Node<V, T>* p) // балансировка узла p
    {
        fixHeight(p);
        if (bfactor(p) == 2)
        {
            if (bfactor(p->getRight()) < 0)
            {
                p->setRight(RotateRight(p->getRight()));
                p->getRight()->setParent(p);
            }
            return RotateLeft(p);
        }
        if (bfactor(p) == -2)
        {
            if (bfactor(p->getLeft()) > 0)
            {
                p->setLeft(RotateLeft(p->getLeft()));
                p->getLeft()->setParent(p);
            }
            return RotateRight(p);
        }

        return p; // балансировка не нужна
    }



public:
    //конструктор AVL_Tree вызывает конструктор базового класса Tree
    AVL_Tree<V, T>() : IteratedTree<V, T>() {}

    virtual Node<V, T>* push_R(Node<V, T>* N)
    {
        return push_R(N, Tree<V, T>::root);
    }

    //рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
    virtual Node<V, T>* push_R(Node<V, T>* N, Node<V, T>* Current)
    {
        //вызываем функцию push_R из базового класса
        Node<V, T>* AddedNode = Tree<V, T>::push_R(N, Current);
        //применяем к добавленному узлу балансировку
        return Balance(AddedNode);
    }

    //функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
    virtual Node<V, T>* push(T n)
    {
        Node<V, T>* N = new Node<V, T>;
        N->setData(n);
        return push_R(N);
    }

    //удаление узла
    void Remove(Node<V, T>* N)
    {
        Tree<V, T>::Remove(N);
        Balance(Tree<V, T>::root);
    }
    void RemoveByKey(V key)
    {
        Node<V, T>* tmp = Tree<V, T>::RemoveByKey(key);
        Balance(Tree<V, T>::root);
    }
    void RemoveByValue(T value)
    {
        Node<V, T>* tmp = Tree<V, T>::RemoveByValue(value);
        Balance(Tree<V, T>::root);
    }
    Node<V, T>* getRoot()
    {
        return Tree<V, T>::root;
    }

    Node<V, T>* operator[](int n) // поиск по индексу inorder
    {
        TreeIterator<V, T> it = Tree<V, T>::root;
        for (int i = 0; i < n; ++i) it++;
        if (it != nullptr)
        {
            return *it;
        }
    }

    void PrintForward()
    {
        TreeIterator<V, T> it = this->Min();
        while (it != nullptr)
        {
            cout << (*it)->getData() << endl;
            if (it == this->Max())
                break;
            it++;
        }
    }

    void PrintBackward()
    {
        TreeIterator<V, T> it = this->Max();
        while (it != this->Min())
        {
            cout << (*it)->getData() << endl;
            it--;
        }
        cout << (*it)->getData() << endl;
    }

    Node<V, T>* SearchByData(T a, Node<V, T>* p)
    {
        return Tree<V, T>::Find(a, p);
    }
    Node<V, T>* SearchByKey(V k, Node<V, T>* p)
    {
        if (p == nullptr)
            return nullptr;
        if (p->getkey() == k)
            return p;
        if (p->getRight()->getkey() <= k)
            return SearchByKey(k, p->getRight());
        if (p->getLeft()->getkey() >= k)
            return SearchByKey(k, p->getLeft());
    }

};

template<class V, class T>
class multitree : public AVL_Tree<V, T>
{
private:
    Node<V, T>* push_R5(Node<V, T>* N, Node<V, T>* Current)
    {
        if (N == NULL)
            return NULL;
        if (Tree<V, T>::root == NULL)
        {
            Tree<V, T>::root = N;
            return N;
        }
        if (Current->getData() > N->getData())
        {
            if (Current->getLeft() != NULL)
                Current->setLeft(push_R5(N, Current->getLeft()));
            else
                Current->setLeft(N);
            Current->getLeft()->setParent(Current);
        }
        if ((Current->getData() < N->getData()) || ((Current->getData() == N->getData())))
        {
            if (Current->getRight() != NULL)
                Current->setRight(push_R5(N, Current->getRight()));
            else
                Current->setRight(N);
            Current->getRight()->setParent(Current);
        }
        return Current;
    }
public:
    multitree<V, T>() : AVL_Tree<V, T>() {}

    Node<V, T>* push(T n)
    {
        Node<V, T>* N = new Node<V, T>;
        N->setData(n);
        return push_R4(N);
    }
    Node<V, T>* push_R4(Node<V, T>* N)
    {
        return push_R4(N, Tree<V, T>::root);
    }
    Node<V, T>* push_R4(Node<V, T>* N, Node<V, T>* Current)
    {
        Node<V, T>* AddedNode = this->push_R5(N, Current);
        return AVL_Tree<V, T>::Balance(AddedNode);
    }

    multiset<T> operator[](V key)
    {
        multiset<T> result;
        Node<V, T>* current = Tree<V, T>::root;
        Node<V, T>* res;

        while (current != nullptr)
        {
            Node<V, T>* res = AVL_Tree<V, T>::SearchByKey(key, current);
            current = res->getRight();
            result.insert(res->getData());
        }

        return result;
    }

};


int main()
{
    setlocale(LC_ALL, "rus");

    AVL_Tree<int, HeavenlBody> SpaceTree;

    HeavenlBody first(asteroid, 9, 700000, 1000);
    HeavenlBody second(star, 5, 700000, 1000);
    HeavenlBody third(planet, 2, 700000, 1000);
    HeavenlBody fourth(planet, 100, 700000, 999999);
    HeavenlBody zero(asteroid, 9, 700000, "Inf");

    SpaceTree.push(first);
    SpaceTree.push(second);
    SpaceTree.push(third);
    SpaceTree.push(fourth);

    cout << "\n-------------------------------|" << endl;
    cout << "Вывод в порядке убывания номера|" << endl;
    cout << "-------------------------------|\n" << endl;

    SpaceTree.PrintBackward();
    cout << endl << endl;

    cout << "\n----------------------------------|" << endl;
    cout << "Вывод в порядке возрастания номера|" << endl;
    cout << "----------------------------------|\n" << endl;

    SpaceTree.PrintForward();
    cout << endl << endl;


    cout << "\n--------------------|" << endl;
    cout << "Удаление по значению|" << endl;
    cout << "--------------------|\n" << endl;
    SpaceTree.RemoveByValue(third);
    SpaceTree.PrintBackward();
    cout << endl << endl;


    cout << "\n-----------------|" << endl;
    cout << "Поиск по значению|" << endl;
    cout << "-----------------|\n" << endl;
    cout << *(SpaceTree.SearchByData(first, SpaceTree.getRoot()));
    cout << endl << endl;


    cout << "\n--------------|" << endl;
    cout << "Поиск по ключу|" << endl;
    cout << "--------------|\n" << endl;
    cout << *(SpaceTree.SearchByKey(9, SpaceTree.getRoot()));
    cout << endl << endl;


    cout << "\n-----------------------------------------------------|" << endl;
    cout << "Обращение к итератору, который не на что не указывает|" << endl;
    cout << "-----------------------------------------------------|\n" << endl;

    TreeIterator<int, HeavenlBody> it = SpaceTree.begin();
    it--;
    try
    {
        cout << (*it)->getData();
    }
    catch (int value)
    {
        cout << "Ошибка! " << endl;
    }
    cout << endl << endl;


    cout << "\n---------------------------------------|" << endl;
    cout << "Несколько элементов с одинаковым ключом|" << endl;
    cout << "---------------------------------------|\n" << endl;

    HeavenlBody six(star, 5, 600000, 500);
    multitree<int, HeavenlBody> multi_space_tree;

    multi_space_tree.push(first);
    multi_space_tree.push(second);
    multi_space_tree.push(fourth);
    multi_space_tree.push(third);
    multi_space_tree.push(six);
    multi_space_tree.PrintBackward();


    cout << "\n----------|" << endl;
    cout << "Оператор[]|" << endl;
    cout << "----------|\n" << endl;

    multiset<HeavenlBody> result = multi_space_tree[5];
    for (auto i : result)
        cout << i << endl;

    cout << "\n----------------------------------------------------|" << endl;
    cout << "Перебор всех элементов с начала, с помощью итераторов|" << endl;
    cout << "----------------------------------------------------|\n" << endl;

    TreeIterator<int, HeavenlBody> it1 = multi_space_tree.begin();
    while (it1 != NULL)
    {
        cout << *(*it1) << endl;
        it1++;
    }

    cout << "\n----------------------------------------------------|" << endl;
    cout << "Перебор всех элементов с конца, с помощью итераторов|" << endl;
    cout << "----------------------------------------------------|\n" << endl;

    TreeIterator<int, HeavenlBody> it2 = multi_space_tree.end();
    while (it2 != NULL)
    {
        cout << *(*it2) << endl;
        it2--;
    }

    return 0;
}

