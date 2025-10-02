#include "VectorStore.h"

// ----------------- ArrayList Implementation -----------------

template <class T>
ArrayList<T>::ArrayList(int initCapacity) {
    capacity = (initCapacity > 0 ? initCapacity : 10);
    count = 0;
    data = new T[capacity];
}

template <class T>
ArrayList<T>::ArrayList(const ArrayList<T>& other) {
    capacity = other.capacity;
    count = other.count;
    data = new T[capacity];
    for (int i = 0; i < count; i++) data[i] = other.data[i];
}   

template <class T>
ArrayList<T>::~ArrayList() {
    delete[] data;
}

template <class T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& other) {
    if (this == &other) return *this;
    delete[] data;
    capacity = other.capacity;
    count = other.count;
    data = new T[capacity];
    for (int i = 0; i < count; i++) data[i] = other.data[i];
    return *this;
}

template <class T>
void ArrayList<T>::ensureCapacity(int cap) {
    if (cap <= capacity) return;
    int newCap = (capacity * 3) / 2 + 1;
    if (newCap < cap) newCap = cap;
    T* newData = new T[newCap];
    for (int i = 0; i < count; i++) newData[i] = data[i];
    delete[] data;
    data = newData;
    capacity = newCap;
}

template <class T>
void ArrayList<T>::add(T e) {
    ensureCapacity(count + 1);
    data[count++] = e;
}

template <class T>
void ArrayList<T>::add(int index, T e) {
    if (index < 0 || index > count) throw out_of_range("Index is invalid!");
    ensureCapacity(count + 1);
    for (int i = count; i > index; i--) data[i] = data[i-1];
    data[index] = e;
    count++;
}

template <class T>
T ArrayList<T>::removeAt(int index) {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    T val = data[index];
    for (int i = index; i < count-1; i++) data[i] = data[i+1];
    count--;
    return val;
}

template <class T>
bool ArrayList<T>::empty() const { return count == 0; }

template <class T>
int ArrayList<T>::size() const { return count; }

template <class T>
void ArrayList<T>::clear() {
    delete[] data;
    capacity = 10;
    count = 0;
    data = new T[capacity];
}

template <class T>
T& ArrayList<T>::get(int index) {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    return data[index];
}
template <class T>
const T& ArrayList<T>::get(int index) const {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    return data[index];
}

template <class T>
void ArrayList<T>::set(int index, T e) {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    data[index] = e;
}

template <class T>
int ArrayList<T>::indexOf(T item) const {
    for (int i = 0; i < count; i++) if (data[i] == item) return i;
    return -1;
}

template <class T>
bool ArrayList<T>::contains(T item) const { return indexOf(item) != -1; }

template <class T>
string ArrayList<T>::toString(string (*item2str)(T&)) const {
    ostringstream oss;
    oss << "[";
    for (int i = 0; i < count; i++) {
        if (i > 0) oss << ", ";
        if (item2str) oss << item2str(data[i]);
        else oss << data[i];
    }
    oss << "]";
    return oss.str();
}

// ----------------- Iterator of ArrayList Implementation -----------------
template <class T>
ArrayList<T>::Iterator::Iterator(ArrayList<T>* pList, int index) {
    if (!pList) { this->pList = nullptr; cursor = 0; return; }
    if (index < 0 || index > pList->count) throw out_of_range("Index is invalid!");
    this->pList = pList;
    cursor = index;
}

template <class T>
typename ArrayList<T>::Iterator& ArrayList<T>::Iterator::operator=(const Iterator& other) {
    pList = other.pList;
    cursor = other.cursor;
    return *this;
}

template <class T>
T& ArrayList<T>::Iterator::operator*() {
    if (!pList || cursor >= pList->count) throw out_of_range("Iterator is out of range!");
    return pList->data[cursor];
}

template <class T>
bool ArrayList<T>::Iterator::operator!=(const Iterator& other) const {
    return (pList != other.pList) || (cursor != other.cursor);
}

template <class T>
typename ArrayList<T>::Iterator& ArrayList<T>::Iterator::operator++() {
    if (!pList || cursor >= pList->count) throw out_of_range("Iterator cannot advance past end!");
    cursor++;
    return *this;
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator++(int) {
    Iterator old = *this;
    ++(*this);
    return old;
}

template <class T>
typename ArrayList<T>::Iterator& ArrayList<T>::Iterator::operator--() {
    if (!pList || cursor <= 0) throw out_of_range("Iterator cannot move before begin!");
    cursor--;
    return *this;
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator--(int) {
    Iterator old = *this;
    --(*this);
    return old;
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::begin() { return Iterator(this, 0); }

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::end() { return Iterator(this, count); }

// ----------------- SinglyLinkedList Implementation -----------------
template <class T>
SinglyLinkedList<T>::SinglyLinkedList() {
    head = tail = nullptr;
    count = 0;
}   

template <class T>
SinglyLinkedList<T>::~SinglyLinkedList() { clear(); }   

template <class T>
void SinglyLinkedList<T>::add(T e) {
    Node* node = new Node(e);
    if (!head) head = tail = node;
    else { tail->next = node; tail = node; }
    count++;
}

template <class T>
void SinglyLinkedList<T>::add(int index, T e) {
    if (index < 0 || index > count) throw out_of_range("Index is invalid!");
    if (index == count) { add(e); return; }
    Node* node = new Node(e);
    if (index == 0) {
        node->next = head;
        head = node;
        if (!tail) tail = head;
    } else {
        Node* prev = head;
        for (int i = 0; i < index - 1; i++) prev = prev->next;
        node->next = prev->next;
        prev->next = node;
    }
    count++;
}

template <class T>
T SinglyLinkedList<T>::removeAt(int index) {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    Node* del;
    T val;
    if (index == 0) {
        del = head;
        head = head->next;
        if (tail == del) tail = nullptr;
    } else {
        Node* prev = head;
        for (int i = 0; i < index - 1; i++) prev = prev->next;
        del = prev->next;
        prev->next = del->next;
        if (del == tail) tail = prev;
    }
    val = del->data;
    delete del;
    count--;
    return val;
}

template <class T>
bool SinglyLinkedList<T>::removeItem(T item) {
    Node* prev = nullptr, *cur = head;
    while (cur) {
        if (cur->data == item) {
            if (prev) prev->next = cur->next;
            else head = cur->next;
            if (cur == tail) tail = prev;
            delete cur;
            count--;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

template <class T>
bool SinglyLinkedList<T>::empty() const { return count == 0; }

template <class T>
int SinglyLinkedList<T>::size() const { return count; }

template <class T>
void SinglyLinkedList<T>::clear() {
    Node* cur = head;
    while (cur) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    head = tail = nullptr;
    count = 0;
}

template <class T>
T& SinglyLinkedList<T>::get(int index) {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    Node* cur = head;
    for (int i = 0; i < index; i++) cur = cur->next;
    return cur->data;
}

template <class T>
const T& SinglyLinkedList<T>::get(int index) const {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    Node* cur = head;
    for (int i = 0; i < index; i++) cur = cur->next;
    return cur->data;
}

template <class T>
int SinglyLinkedList<T>::indexOf(T item) const {
    Node* cur = head; int idx=0;
    while (cur) {
        if (cur->data == item) return idx;
        cur = cur->next; idx++;
    }
    return -1;
}

template <class T>
bool SinglyLinkedList<T>::contains(T item) const { return indexOf(item)!=-1; }

template <class T>
string SinglyLinkedList<T>::toString(string (*item2str)(T&)) const {
    ostringstream oss;
    Node* cur = head;
    while (cur) {
        if (item2str) oss << "[" << item2str(cur->data) << "]";
        else oss << "[" << cur->data << "]";
        if (cur->next) oss << "->";
        cur = cur->next;
    }
    return oss.str();
}

template <class T>
SinglyLinkedList<T>::Iterator::Iterator(Node* node) { current = node; }   

template <class T>
typename SinglyLinkedList<T>::Iterator& SinglyLinkedList<T>::Iterator::operator=(const Iterator& other) {
    current = other.current; return *this;
}

template <class T>
T& SinglyLinkedList<T>::Iterator::operator*() {
    if (!current) throw out_of_range("Iterator is out of range!");
    return current->data;
}

template <class T>
bool SinglyLinkedList<T>::Iterator::operator!=(const Iterator& other) const { return current != other.current; }

template <class T>
typename SinglyLinkedList<T>::Iterator& SinglyLinkedList<T>::Iterator::operator++() {
    if (!current) throw out_of_range("Iterator cannot advance past end!");
    current = current->next;
    return *this;
}

template <class T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::Iterator::operator++(int) {
    Iterator old = *this;
    ++(*this);
    return old;
}

// ----------------- VectorStore Implementation -----------------

VectorStore::VectorStore(int dimension, EmbedFn embeddingFunction) {
    this->dimension = dimension;
    this->embeddingFunction = embeddingFunction;
    this->count = 0;
}

VectorStore::~VectorStore() { clear(); }

void VectorStore::clear() {
    for (int i = 0; i < records.size(); i++) {
        delete records.get(i)->vector;
        delete records.get(i);
    }
    records.clear();
    count = 0;
}

// ----------------- VectorStore Implementation -----------------

VectorStore::VectorRecord::VectorRecord(int id, const string& rawText, SinglyLinkedList<float>* vector) {
    this->id = id;
    this->rawText = rawText;
    this->rawLength = rawText.size();
    this->vector = vector;
}

VectorStore::VectorStore(int dimension, EmbedFn embeddingFunction) {
    this->dimension = dimension;
    this->embeddingFunction = embeddingFunction;
    this->count = 0;
}

VectorStore::~VectorStore() { clear(); }

int VectorStore::size() const { return count; }

bool VectorStore::empty() const { return count == 0; }

void VectorStore::clear() {
    for (int i = 0; i < records.size(); i++) {
        delete records.get(i)->vector;
        delete records.get(i);
    }
    records.clear();
    count = 0;
}

SinglyLinkedList<float>* VectorStore::preprocessing(string rawText) {
    SinglyLinkedList<float>* vec = embeddingFunction ? embeddingFunction(rawText) : new SinglyLinkedList<float>();
    int d = vec->size();
    if (d > dimension) {
        while (vec->size() > dimension) vec->removeAt(vec->size() - 1);
    } else if (d < dimension) {
        while (vec->size() < dimension) vec->add(0.0f);
    }
    return vec;
}

void VectorStore::addText(string rawText) {
    SinglyLinkedList<float>* vec = preprocessing(rawText);
    VectorRecord* rec = new VectorRecord(count + 1, rawText, vec);
    records.add(rec);
    count++;
}

SinglyLinkedList<float>& VectorStore::getVector(int index) {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    return *(records.get(index)->vector);
}

string VectorStore::getRawText(int index) const {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    return records.get(index)->rawText;
}

int VectorStore::getId(int index) const {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    return records.get(index)->id;
}

bool VectorStore::removeAt(int index) {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    VectorRecord* rec = records.removeAt(index);
    delete rec->vector;
    delete rec;
    count--;
    return true;
}

bool VectorStore::updateText(int index, string newRawText) {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    VectorRecord* rec = records.get(index);
    delete rec->vector;
    rec->rawText = newRawText;
    rec->rawLength = newRawText.size();
    rec->vector = preprocessing(newRawText);
    return true;
}

void VectorStore::setEmbeddingFunction(EmbedFn newEmbeddingFunction) {
    embeddingFunction = newEmbeddingFunction;
}

void VectorStore::forEach(void (*action)(SinglyLinkedList<float>&, int, string&)) {
    for (int i = 0; i < count; i++) {
        VectorRecord* rec = records.get(i);
        action(*(rec->vector), rec->id, rec->rawText);
    }
}

double VectorStore::cosineSimilarity(const SinglyLinkedList<float>& v1,
                                     const SinglyLinkedList<float>& v2) const {
    double dot = 0, norm1 = 0, norm2 = 0;
    for (int i = 0; i < dimension; i++) {
        float a = ((SinglyLinkedList<float>&)v1).get(i);
        float b = ((SinglyLinkedList<float>&)v2).get(i);
        dot += a * b;
        norm1 += a * a;
        norm2 += b * b;
    }
    if (norm1 == 0 || norm2 == 0) return 0.0;
    return dot / (sqrt(norm1) * sqrt(norm2));
}

double VectorStore::l1Distance(const SinglyLinkedList<float>& v1,
                               const SinglyLinkedList<float>& v2) const {
    double sum = 0;
    for (int i = 0; i < dimension; i++) {
        float a = ((SinglyLinkedList<float>&)v1).get(i);
        float b = ((SinglyLinkedList<float>&)v2).get(i);
        sum += fabs(a - b);
    }
    return sum;
}

double VectorStore::l2Distance(const SinglyLinkedList<float>& v1,
                               const SinglyLinkedList<float>& v2) const {
    double sum = 0;
    for (int i = 0; i < dimension; i++) {
        float a = ((SinglyLinkedList<float>&)v1).get(i);
        float b = ((SinglyLinkedList<float>&)v2).get(i);
        sum += (a - b) * (a - b);
    }
    return sqrt(sum);
}

int VectorStore::findNearest(const SinglyLinkedList<float>& query, const string& metric) const {
    if (count == 0) return -1;
    double bestScore;
    if (metric == "cosine") bestScore = -2;
    else bestScore = 1e18;
    int bestIdx = -1;

    for (int i = 0; i < count; i++) {
        double score;
        if (metric == "cosine")
            score = cosineSimilarity(query, *(records.get(i)->vector));
        else if (metric == "euclidean")
            score = l2Distance(query, *(records.get(i)->vector));
        else if (metric == "manhattan")
            score = l1Distance(query, *(records.get(i)->vector));
        else
            throw invalid_metric();

        if (metric == "cosine") {
            if (score > bestScore) { bestScore = score; bestIdx = i; }
        } else {
            if (score < bestScore) { bestScore = score; bestIdx = i; }
        }
    }
    return bestIdx;
}

int* VectorStore::topKNearest(const SinglyLinkedList<float>& query, int k, const string& metric) const {
    if (k <= 0 || k > count) throw invalid_k_value();

    // Tạo mảng 2 chiều: [score, index]
    struct Item { double score; int idx; };
    Item* arr = new Item[count];

    for (int i = 0; i < count; i++) {
        double score;
        if (metric == "cosine")
            score = cosineSimilarity(query, *(records.get(i)->vector));
        else if (metric == "euclidean")
            score = l2Distance(query, *(records.get(i)->vector));
        else if (metric == "manhattan")
            score = l1Distance(query, *(records.get(i)->vector));
        else
            throw invalid_metric();

        // với cosine: càng lớn càng tốt (đảo dấu để sort asc)
        if (metric == "cosine") arr[i] = {-score, i};
        else arr[i] = {score, i};
    }

    // sort thủ công (bubble sort đơn giản cho dễ hiểu)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (arr[i].score > arr[j].score) {
                Item tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }

    int* result = new int[k];
    for (int i = 0; i < k; i++) result[i] = arr[i].idx;

    delete[] arr;
    return result;
}


// Explicit template instantiation for char, string, int, double, float, and Point

template class ArrayList<char>;
template class ArrayList<string>;
template class ArrayList<int>;
template class ArrayList<double>;
template class ArrayList<float>;
template class ArrayList<Point>;

template class SinglyLinkedList<char>;
template class SinglyLinkedList<string>;
template class SinglyLinkedList<int>;
template class SinglyLinkedList<double>;
template class SinglyLinkedList<float>;
template class SinglyLinkedList<Point>;
