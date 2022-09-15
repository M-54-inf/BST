#pragma once

template <class MyData, class MyKey>
class MyTree {

protected:
	class MyNode {
	public:
		MyKey Key;
		MyData Data;
		MyNode* Left;
		MyNode* Right;
		int n;
		
		MyNode(MyKey K, MyData D) {
			Key = K;
			Data = D;
			Left = nullptr;
			Right = nullptr;
			n = 0;
		}
	};
	friend class MyNode;

private:
	MyNode* TreeRoot;
	int NumberOfNodes;

public:
	
	class Iterator
	{
	private:
		MyTree* Ptr;	//указатель на объект коллекции
		MyNode* Cur;	//указатель на текущий элемент коллекции
	
	public:
		
		//конструктор
		Iterator(MyTree<MyKey, MyData>& Tree) {
			Ptr = &Tree;
			Cur = nullptr;
		}

		//установка на элемент с наименьшим ключом c использованием рекурсии
		void begin() {
			if (Ptr->TreeRoot != nullptr) {
				Cur = go_to_begin(Ptr->TreeRoot);
			}
			else {
				throw - 1;
			}
			return;
		}

		MyNode* go_to_begin(MyNode* Cur) {
			if(Cur->Left!=nullptr) {
				return go_to_begin(Cur->Left);
			}
		}

		//установка на элемент с наибольшим ключом c использованием рекурсии
		void end() {
			if (Ptr->TreeRoot != nullptr) {
				Cur = go_to_end(Ptr->TreeRoot);
			}
			else {
				throw - 1;
			}
			return;
		}

		MyNode* go_to_end(MyNode* Cur) {
			if(Cur->Right!=nullptr) {
				return go_to_end(Cur->Right);
			}
		}

		//установка на следующий
		void next() {
			if (Cur != nullptr) {
				Cur = Ptr->BST_Successor(Ptr->TreeRoot, Cur);
			}
			else {
				throw - 1;
			}
			return;
		}

		//установка на предыдущий
		void prev() {
			if (Cur != nullptr) {
			Cur = Ptr->BST_Predcessor(Ptr->TreeRoot, Cur);
			}
			else {
				throw - 1;
			}
			return;
		}

		//проверка состояния итератора
		bool is_off() const {
			return (Cur == nullptr);
		}

		//доспуп к данным текущего элемента
		MyData& operator*() {
			if (Cur != nullptr)
				return Cur->Data;
			else
				throw -1;
		}
	};
	friend class Iterator;

	class ReverseIterator
	{
	private:
		MyTree* Ptr;	//указатель на объект коллекции
		MyNode* Cur;	//указатель на текущий элемент коллекции
	public:
		//конструктор
		ReverseIterator(MyTree<MyKey, MyData>& Tree) {
			Ptr = &Tree;
			Cur = nullptr;
		}

		//установка на элемент с наименьшим ключом c использованием рекурсии
		void rbegin() {
			if (Ptr->TreeRoot != nullptr) {
				Cur = go_to_rbegin(Ptr->TreeRoot);
			}
			else {
				throw - 1;
			}
			return;
		}

		MyNode* go_to_rbegin(MyNode* Cur) {
			if (Cur->Left != nullptr){
				return go_to_rbegin(Cur->Left);
			}
		}

		//установка на элемент с наибольшим ключом c использованием рекурсии
		void rend() {
			if (Ptr->TreeRoot != nullptr) {
				Cur = go_to_rend(Ptr->TreeRoot);
			}
			else {
				throw - 1;
			}
			return;
		}

		MyNode* go_to_rend(MyNode* Cur) {
			if (Cur->Right != nullptr) {
				return go_to_rend(Cur->Right);
			}
		}

		//проверка состояния итератора
		bool is_off() const {
			return (Cur == nullptr);
		}

		//установка на следующий по ключу
		void next() {
			if (Cur != nullptr) {
				Cur = Ptr->BST_Successor(Ptr->TreeRoot, Cur);
			}
			else {
				throw - 1;
			}
			return;
		}

		//установка на предыдущий по ключу
		void prev() {
			if (Cur != nullptr) {
				Cur = Ptr->BST_Predcessor(Ptr->TreeRoot, Cur);
			}
			else {
				throw - 1;
			}
			return;
		}

		//доспуп к данным текущего элемента
		MyData& operator*() {
			if (Cur != nullptr)
				return Cur->Data;
			else
				throw - 1;
		}
	};
	friend class ReverseIterator;

	MyTree() {//конструктор
		TreeRoot = nullptr;
		NumberOfNodes = 0;
	}

	MyTree(const MyTree<MyData, MyKey>& OldTree) {//конструктор копирования
		TreeRoot = nullptr;
		NumberOfNodes = 0;
		RecursionCopy(TreeRoot, OldTree->TreeRoot);
	}

	void RecursionCopy(MyNode* Root, MyNode*& OldRoot) {//функция рекурсивного копирования, вызываемая конструктором копирования
		if (Root == nullptr) {
			return;
		}
		bool b =false;
		int n = 0;
		if (BST_Insert(Root, OldRoot->Key, OldRoot->Data, b, n) == false){
			throw - 1;
		};
		RecursionCopy(Root->Left, OldRoot->Left);
		RecursionCopy(Root->Right, OldRoot->Left);
	}

	~MyTree() {//деструктор
		DeleteAll();
	}

	void DeleteAll() {
		RecursionClear(TreeRoot);
		TreeRoot = nullptr;
		NumberOfNodes = 0;
	}

	void RecursionClear(MyNode* Root) {//очистка дерева
		if (Root == nullptr) {
			return;
		}
		MyNode* LTree = Root->Left;
		MyNode* RTree = Root->Right;
		delete Root;
		RecursionClear(LTree);
		RecursionClear(RTree);
	}

	int AskForSize() {//опрос размера списка
		return NumberOfNodes;
	}

	bool EmptyCheck() {//проверка на пустоту
		return(NumberOfNodes == 0);
	}

	//доступ по чтению к данным по ключу
	MyData Read(MyKey Key, int& looked) {
		return BST_Read(TreeRoot, Key, looked);
	}

	MyData BST_Read(MyNode* Root, MyKey Key, int& looked) {
		if (Root == nullptr) {
			throw - 1;
		}
		else {
			if (Key == Root->Key) {
				return Root->Data;
			}
			if (Root->Key > Key) {
				looked++;
				return BST_Read(Root->Left, Key, looked);
			}
			else {
				looked++;
				return BST_Read(Root->Right, Key, looked);
			}
		}
	}
	
	//вставка элемента по ключу
	bool Insert(MyKey Key, MyData Data, bool& inserted, int& looked) {
		BST_Insert(TreeRoot, Key, Data, inserted, looked);
		if (inserted == true) {
			return true;
		}
		else {
			return false;
		}
	}

	MyNode* BST_Insert(MyNode*& Root, MyKey Key, MyData Data, bool &inserted, int& looked) {
		bool ins;
		if (Root == nullptr) {
			inserted = true;
			Root = new MyNode(Key, Data);
			NumberOfNodes++;
			return Root;
		}
		else {
			if (Root->Key == Key) {
				inserted = false;
				return go_back_ins(TreeRoot, Key);
			}
			if (Key < Root->Key) {
				looked++;
				Root->n++;
				Root->Left = BST_Insert(Root->Left, Key, Data, ins, looked);
			}
			else if (Key > Root->Key) {
				looked++;
				Root->n++;
				Root->Right = BST_Insert(Root->Right, Key, Data, ins, looked);
			}
		}
		inserted = ins;
		return Root;
	}

	//Изменение значения элемента с заданным ключом
	bool InsertNewData(MyKey Key, MyData Data) {
		return  KeyInsert(TreeRoot, Key, Data);
	}

	bool KeyInsert(MyNode* Root, MyKey Key, MyData Data) {
		if (Root == nullptr) {
			return false;
		}
		if (Root->Key == Key) {
			Root->Data == Data;
			return true;
		}
		if (Root->Key > Key) {
			KeyInsert(Root->Left, Key, Data);
		}
		if (Root->Key < Key) {
			KeyInsert(Root->Left, Key, Data);
		}
	}

	//удаление элемента с заданным ключом
	bool Delete(MyKey Key, bool& deleted, int& looked) {
		BST_Delete(TreeRoot, Key, deleted, looked);
		if (deleted == true) {
			return true;
		}
		else {
			return false;
		}
	}

	MyNode* BST_Delete(MyNode*& Root, MyKey Key, bool &deleted, int&looked) {
		MyNode* x;
		bool del;
		if (Root == nullptr) {
			deleted = false;
			return go_back_del(TreeRoot, Key);
		}
		if (Key < Root->Key) {
			looked++;
			Root->n--;
			Root->Left = BST_Delete(Root->Left, Key, del, looked);
			deleted = del;
			return Root;
		}
		if (Key > Root->Key) {
			looked++;
			Root->n--;
			Root->Right = BST_Delete(Root->Right, Key, del, looked);
			deleted = del;
			return Root;
		}				
		deleted = true;
		if ((Root->Left == nullptr) && (Root->Right == nullptr)) {
			Delete_Node(Root);
			return nullptr;
			}
		if (Root->Left == nullptr){
			x = Root->Right;
			Delete_Node(Root);
			return x;
			}
		if (Root->Right == nullptr) {
			x = Root->Left;
			Delete_Node(Root);
			return x;
			}
		Root->Right = Del(Root->Right, Root);
		Root->n = 0;
		if (Root->Left != nullptr) {
			//std::cout << Root->n << " " << Root->Left->n << std::endl;
			Root->n += Root->Left->n + 1;
		}
		if (Root->Right != nullptr) {
			//std::cout << Root->n << " " << Root->Right->n << std::endl;
			Root->n += Root->Right->n + 1;
		}
		return Root;
	}

	//техническая функция для удаления элемента с двумя потомками
	MyNode* Del(MyNode* Root, MyNode* t0) {
		MyNode* x = BST_Predcessor(TreeRoot, t0);
		if (Root->Left != nullptr) {
			Root->n--;
			Root->Left = Del(Root->Left, t0);
			return Root;
		}
		t0->Key = Root->Key;
		t0->Data = Root->Data;
		x = Root->Right;
		Delete_Node(Root);
		return x;
	}

	void Delete_Node(MyNode* Root) {
		delete Root;
		NumberOfNodes--;
	}

	MyNode* go_back_del(MyNode* Root, MyKey Key) {
		if (Root == nullptr) {
			return Root;
		}
		if (Root->Key < Key) {
			Root->n++;
			return go_back_del(Root->Right, Key);
		}
		if (Root->Key > Key) {
			Root->n++;
			return go_back_del(Root->Left, Key);
		}
	}

	MyNode* go_back_ins(MyNode* Root, MyKey Key) {
		if (Root == nullptr) {
			return Root;
		}
		if (Root->Key < Key) {
			Root->n--;
			return go_back_ins(Root->Right, Key);
		}
		if (Root->Key > Key) {
			Root->n--;
			return go_back_ins(Root->Left, Key);
		}
	}

	void StartPrint() {
		Print(TreeRoot);
	}

	//Вывод ключей в порядке t->L->R
	void Print(MyNode* Root) {
		if (Root == nullptr) {
			return;
		}
		std::cout << Root->Key << std::endl;
		Print(Root->Left);
		Print(Root->Right);
	}

	//определение порядкового номера для элемента с заданным ключом (дополнительная операция)
	int KeyNumber(MyKey Key, int& nn) {
		int n=1;
		return AskForNumber(TreeRoot, Key, n, nn);
	}
	
	int AskForNumber(MyNode* Root, MyKey Key, int Counter, int& nn) {
		if (Root == nullptr) {
			return -1;
		}
		if (Key < Root->Key) {
			nn++;
			return AskForNumber(Root->Left, Key, Counter, nn);
		}
		if (Key > Root->Key) {
			nn++;
			if (Root->Left != nullptr) {
				return AskForNumber(Root->Right, Key, Counter + Root->Left->n + 2, nn);
			}
			else {
				return AskForNumber(Root->Right, Key, Counter + 1, nn);
			}
		}
		if (Root->Left != nullptr) {
			Counter += Root->Left->n + 1;
		}
		return Counter;
	}

	//Запрос о наличии заданного ключа в дереве
	bool KeyCheck(MyKey Key) {
		return AskForKey(TreeRoot, Key);
	}

	bool AskForKey(MyNode* Root, MyKey Key) {
		if (Root == nullptr) {
			return false;
		}
		if (Root->Key == Key) {
			return true;
		}
		else {
			if (Root->Key > Key) {
				AskForKey(Root->Left, Key);
			}
			else {
				AskForKey(Root->Right, Key);
			}
		}
	}

	//Поиск следующего по ключу узла
	MyNode* BST_Successor(MyNode* TRoot, MyNode* Root) {
		if (Root->Right != nullptr) {
			return Min(Root->Right);
		}
		else {
			return L_Successor(TRoot, Root);
		}
	}

	MyNode* Min(MyNode* Root) {
		if (Root == nullptr) {
			return nullptr;
		}
		if (Root->Left != nullptr) {
			return Min(Root->Left);
		}
		return Root;
	}

	MyNode* L_Successor(MyNode* TRoot, MyNode* Root) {
		MyNode* LS;
		if (Root == TRoot) {
			return nullptr;
		} 
		if (Root->Key < TRoot->Key) {
			LS = L_Successor(TRoot->Left, Root);
			if (LS != nullptr) {
				return LS;
			}
			else {
				return TRoot;
			}
		}
		else {
			return L_Successor(TRoot->Right, Root);
		}
	}

	//поиск предыдущего по ключу узла
	MyNode* BST_Predcessor(MyNode* TRoot, MyNode* Root) {
		if (Root->Left!=nullptr) {
			return Max(Root->Left);
		}
		else {
			return R_Parent(TRoot, Root);
		}
	}

	MyNode* Max(MyNode* Root) {
		if(Root == nullptr) {
			return nullptr;
		}
		if(Root->Right!=nullptr) {
			Root = Max(Root->Right);
		}
		return Root;
	}

	MyNode* R_Parent(MyNode* TRoot, MyNode* Root) {
		MyNode* rp;
		if (TRoot == Root) {
			return nullptr;
		}
		if (Root->Key > TRoot->Key) {
			rp = R_Parent(TRoot->Right, Root);
			if (rp != nullptr) {
				return rp;
			}
			else {
				return TRoot;
			}
		}
		else {
			return R_Parent(TRoot->Left, Root);
		}
	}

	void PrintTree() {
		Show(TreeRoot, 0);
	}

	void Show(MyNode *Root, int level) {
		if (Root == nullptr) {
			return;
		}
		Show(Root->Right, level + 1);
		for (int i = 0; i <= 1 * level; i++) {
			std::cout << "	";
		}
		std::cout << Root->Key<<" "<<Root->n<<std::endl;
		Show(Root->Left, level + 1);
	}
};

