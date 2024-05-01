#include <iostream>
#include <string>
using namespace std;

struct Info
{
	string name;
	string region;
	int population;
	Info(string n, string r, int p) : name(n), region(r), population(p) {}
};

template<typename T>
class List
{
public:
	List() : head(nullptr), tail(nullptr), size(0), cachedNode(nullptr), cachedIndex(-1) {}
	~List()
	{
		clear();
	}

	void add(T data)
	{
		Node* newnode = new Node(data);
		if (size == 0)
		{
			head = tail = newnode;
		}
		else
		{
			tail->next = newnode;
			newnode->prev = tail;
			tail = newnode;
		}
		size++;
	}

	void insert(int index, T data)
	{
		if (index < 0 || index > size)
		{
			return;
		}
		if (index == size)
		{
			add(data);
			return;
		}
		Node* newnode = new Node(data);
		if (index == 0)
		{
			newnode->next = head;
			head->prev = newnode;
			head = newnode;
		}
		else
		{
			Node* current = getIndex(index);
			newnode->next = current;
			newnode->prev = current->prev;
			current->prev->next = newnode;
			current->prev = newnode;
		}
		cachedIndex = index;
		cachedNode = newnode;
		size++;
	}

	void removeAt(int index)
	{
		if (index < 0 || index >= size) return;
		Node* todel;
		if (index == 0)
		{
			todel = head;
			head = head->next;
			if (head != nullptr)
			{
				head->prev = nullptr;
			}
			else
			{
				tail = nullptr;
			}
		}
		else
		{
			todel = getIndex(index);
			if (todel == tail)
			{
				tail = tail->prev;
				tail->next = nullptr;
			}
			else
			{
			todel->next->prev = todel->prev;
			todel->prev->next = todel->next;
			}
		}
		if (todel == cachedNode)
		{
			cachedNode = todel->prev;
			cachedIndex = max(0, index - 1);
		}
		else if (index < cachedIndex)
		{
			cachedIndex--;
		}
		delete todel;
		size--;
		if (size == 0)
		{
			cachedNode = nullptr;
			cachedIndex = -1;
		}
	}

	T elementAt(int index)
	{
		if (index < 0 || index >= size) throw out_of_range("Index out of range");
		Node* node = getIndex(index);
		return node->data;
	}

	int count()
	{
		return size;
	}

	void clear()
	{
		Node* current = head;
		while (current != nullptr)
		{
			Node* next = current->next;
			delete current;
			current = next;
		}
		head = tail = cachedNode = nullptr;
		size = 0;
		cachedIndex = -1;
	}

	void setElementAt(int index, T newData)
	{
		Node* current = getIndex(index);
		if (current != nullptr) 
		{
			current->data = newData;
			cachedNode = current;
			cachedIndex = index;
		}
	}


private:
	class Node
	{
	public:
		T data;
		Node* next;
		Node* prev;
		Node(T data) : data(data), next(nullptr), prev(nullptr) {}
	};
	Node* head;
	Node* tail;
	int size;
	Node* cachedNode;
	int cachedIndex;

	Node* reach_index(Node* startNode, int start, int end) 
	{
		Node* current = startNode;
		if (start < end) 
		{
			for (int i = start; i < end; ++i) 
			{
				current = current->next;
			}
		}
		else 
		{
			for (int i = start; i > end; --i) 
			{
				current = current->prev;
			}
		}
		return current;
	}

	Node* getIndex(int index) 
	{
		if (index < 0 || index >= size) return nullptr;
		Node* current = nullptr;
		if (cachedNode && abs(cachedIndex - index) <= index && abs(cachedIndex - index) <= size - index - 1) 
		{
			current = reach_index(cachedNode, cachedIndex, index);
		}
		else if (index <= size - index - 1) 
		{
			current = reach_index(head, 0, index);
		}
		else 
		{
			current = reach_index(tail, size - 1, index);
		}
		cachedNode = current;
		cachedIndex = index;
		return current;
	}
};

void removeCitiesRegion(List<Info>& list, const string& region) 
{
	int i = 0;
	while (i < list.count()) 
	{
		Info info = list.elementAt(i);
		if (info.region == region) 
		{
			list.removeAt(i);
		}
		else 
		{
			i++; 
		}
	}
}

void SortedRegions(List<Info>& list) 
{
	List<Info> temp; 
	for (int i = 0; i < list.count(); i++) 
	{
		Info currentCity = list.elementAt(i);
		bool regionFound = false;

		for (int j = 0; j < temp.count(); j++) 
		{
			Info tempCity = temp.elementAt(j);
			if (tempCity.region == currentCity.region)
			{
				tempCity.population += currentCity.population;
				temp.setElementAt(j, tempCity); 
				regionFound = true;
				break;
			}
		}
		if (!regionFound) 
		{
			temp.add(currentCity);
		}
	}
	bool swapped;
	do {
		swapped = false;
		for (int i = 0; i < temp.count() - 1; i++) 
		{
			Info current = temp.elementAt(i);
			Info next = temp.elementAt(i + 1);
			if (current.population < next.population) 
			{
				temp.setElementAt(i, next);
				temp.setElementAt(i + 1, current);
				swapped = true;
			}
		}
	} while (swapped);

	for (int i = 0; i < temp.count(); ++i)
	{
		Info info = temp.elementAt(i);
		cout << "Region: " << info.name << ", Total Population: " << info.population << endl;
	}
}

int main()
{
	List<Info> list;
	int choice;
	do {
		cout << "1. Add info. " << endl;
		cout << "2. Insert info by index. " << endl;
		cout << "3. Delete info by index. " << endl;
		cout << "4. Show info by index. " << endl;
		cout << "5. Count all elements. " << endl;
		cout << "6. Delete all info. " << endl;
		cout << "7. Remove cities by region. " << endl;
		cout << "8. Show regions sorted by population. " << endl;
		cout << "0. Exit. " << endl;
		cin >> choice;
		string name, region;
		int population, index;
		switch (choice)
		{
		case 1:
			cout << "Name of the city: " << endl;
			cin >> name;
			cout << "Name of the region: " << endl;
			cin >> region;
			cout << "Enter the population: " << endl;
			cin >> population;
			list.add(Info(name, region, population));
			cout << "Info is added." << endl;
			break;
		case 2:
			cout << "Enter the index: " << endl;
			cin >> index;
			cout << "Enter the name of the city: " << endl;
			cin >> name;
			cout << "Enter the name of the region: " << endl;
			cin >> region;
			cout << "Enter the population: " << endl;
			cin >> population;
			list.insert(index, Info(name, region, population));
			cout << "Info is inserted. " << endl;
			break;
		case 3:
			cout << "Enter the index: " << endl;
			cin >> index;
			list.removeAt(index);
			cout << "Info is removed. " << endl;
			break;
		case 4:
			cout << "Enter the index: " << endl;
			cin >> index;
			try {
				Info info = list.elementAt(index);
				cout << "City name: " << info.name << ", region name: " << info.region << ", population: " << info.population << endl;
			}
			catch (const out_of_range&) {
				cout << "Could not find a city at that index. " << endl;
			}
			break;
		case 5:
			cout << "Number of elements: " << list.count() << endl;
			break;
		case 6:
			list.clear();
			cout << "All info is deleted" << endl;
			break;
		case 7:
			cout << "Enter the region name: " << endl;
			cin >> region;
			removeCitiesRegion(list, region);
			break;
		case 8:
			cout << " Regions sorted by population: " << endl;
			SortedRegions(list);
			break;
		case 0:
			break;
		default:
			cout << "An unknown command. Try again. " << endl;
			break;
		}
	} while (choice != 0);
}