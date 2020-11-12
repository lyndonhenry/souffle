
#include "souffle/CompiledSouffle.h"

extern "C" {
}

namespace souffle {
static const RamDomain RAM_BIT_SHIFT_MASK = RAM_DOMAIN_SIZE - 1;
struct t_btree_iii__0_1_2__100__111 {
using t_tuple = Tuple<RamDomain, 3>;
struct t_comparator_0{
 int operator()(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0])) ? -1 : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0])) ? 1 :((ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1])) ? -1 : (ramBitCast<RamSigned>(a[1]) > ramBitCast<RamSigned>(b[1])) ? 1 :((ramBitCast<RamSigned>(a[2]) < ramBitCast<RamSigned>(b[2])) ? -1 : (ramBitCast<RamSigned>(a[2]) > ramBitCast<RamSigned>(b[2])) ? 1 :(0)));
 }
bool less(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]))|| (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0])) && ((ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1]))|| (ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1])) && ((ramBitCast<RamSigned>(a[2]) < ramBitCast<RamSigned>(b[2]))));
 }
bool equal(const t_tuple& a, const t_tuple& b) const {
return (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0]))&&(ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1]))&&(ramBitCast<RamSigned>(a[2]) == ramBitCast<RamSigned>(b[2]));
 }
};
using t_ind_0 = btree_set<t_tuple,t_comparator_0>;
t_ind_0 ind_0;
using iterator = t_ind_0::iterator;
struct context {
t_ind_0::operation_hints hints_0_lower;
t_ind_0::operation_hints hints_0_upper;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
if (ind_0.insert(t, h.hints_0_lower)) {
return true;
} else return false;
}
bool insert(const RamDomain* ramDomain) {
RamDomain data[3];
std::copy(ramDomain, ramDomain + 3, data);
const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
context h;
return insert(tuple, h);
}
bool insert(RamDomain a0,RamDomain a1,RamDomain a2) {
RamDomain data[3] = {a0,a1,a2};
return insert(data);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(t, h.hints_0_lower);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(t, h.hints_0_lower);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> lowerUpperRange_000(const t_tuple& /* lower */, const t_tuple& /* upper */, context& /* h */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> lowerUpperRange_000(const t_tuple& /* lower */, const t_tuple& /* upper */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<t_ind_0::iterator> lowerUpperRange_100(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_100(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_100(lower,upper,h);
}
range<t_ind_0::iterator> lowerUpperRange_111(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp == 0) {
    auto pos = ind_0.find(lower, h.hints_0_lower);
    auto fin = ind_0.end();
    if (pos != fin) {fin = pos; ++fin;}
    return make_range(pos, fin);
}
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_111(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_111(lower,upper,h);
}
bool empty() const {
return ind_0.empty();
}
std::vector<range<iterator>> partition() const {
return ind_0.getChunks(400);
}
void purge() {
ind_0.clear();
}
iterator begin() const {
return ind_0.begin();
}
iterator end() const {
return ind_0.end();
}
void printStatistics(std::ostream& o) const {
o << " arity 3 direct b-tree index 0 lex-order [0,1,2]\n";
ind_0.printStats(o);
}
};
struct t_btree_ii__0_1__11__10 {
using t_tuple = Tuple<RamDomain, 2>;
struct t_comparator_0{
 int operator()(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0])) ? -1 : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0])) ? 1 :((ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1])) ? -1 : (ramBitCast<RamSigned>(a[1]) > ramBitCast<RamSigned>(b[1])) ? 1 :(0));
 }
bool less(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]))|| (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0])) && ((ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1])));
 }
bool equal(const t_tuple& a, const t_tuple& b) const {
return (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0]))&&(ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1]));
 }
};
using t_ind_0 = btree_set<t_tuple,t_comparator_0>;
t_ind_0 ind_0;
using iterator = t_ind_0::iterator;
struct context {
t_ind_0::operation_hints hints_0_lower;
t_ind_0::operation_hints hints_0_upper;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
if (ind_0.insert(t, h.hints_0_lower)) {
return true;
} else return false;
}
bool insert(const RamDomain* ramDomain) {
RamDomain data[2];
std::copy(ramDomain, ramDomain + 2, data);
const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
context h;
return insert(tuple, h);
}
bool insert(RamDomain a0,RamDomain a1) {
RamDomain data[2] = {a0,a1};
return insert(data);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(t, h.hints_0_lower);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(t, h.hints_0_lower);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> lowerUpperRange_00(const t_tuple& /* lower */, const t_tuple& /* upper */, context& /* h */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> lowerUpperRange_00(const t_tuple& /* lower */, const t_tuple& /* upper */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<t_ind_0::iterator> lowerUpperRange_11(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp == 0) {
    auto pos = ind_0.find(lower, h.hints_0_lower);
    auto fin = ind_0.end();
    if (pos != fin) {fin = pos; ++fin;}
    return make_range(pos, fin);
}
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_11(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_11(lower,upper,h);
}
range<t_ind_0::iterator> lowerUpperRange_10(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_10(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_10(lower,upper,h);
}
bool empty() const {
return ind_0.empty();
}
std::vector<range<iterator>> partition() const {
return ind_0.getChunks(400);
}
void purge() {
ind_0.clear();
}
iterator begin() const {
return ind_0.begin();
}
iterator end() const {
return ind_0.end();
}
void printStatistics(std::ostream& o) const {
o << " arity 2 direct b-tree index 0 lex-order [0,1]\n";
ind_0.printStats(o);
}
};
struct t_btree_ii__1_0__11__01 {
using t_tuple = Tuple<RamDomain, 2>;
struct t_comparator_0{
 int operator()(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1])) ? -1 : (ramBitCast<RamSigned>(a[1]) > ramBitCast<RamSigned>(b[1])) ? 1 :((ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0])) ? -1 : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0])) ? 1 :(0));
 }
bool less(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1]))|| (ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1])) && ((ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0])));
 }
bool equal(const t_tuple& a, const t_tuple& b) const {
return (ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1]))&&(ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0]));
 }
};
using t_ind_0 = btree_set<t_tuple,t_comparator_0>;
t_ind_0 ind_0;
using iterator = t_ind_0::iterator;
struct context {
t_ind_0::operation_hints hints_0_lower;
t_ind_0::operation_hints hints_0_upper;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
if (ind_0.insert(t, h.hints_0_lower)) {
return true;
} else return false;
}
bool insert(const RamDomain* ramDomain) {
RamDomain data[2];
std::copy(ramDomain, ramDomain + 2, data);
const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
context h;
return insert(tuple, h);
}
bool insert(RamDomain a0,RamDomain a1) {
RamDomain data[2] = {a0,a1};
return insert(data);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(t, h.hints_0_lower);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(t, h.hints_0_lower);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> lowerUpperRange_00(const t_tuple& /* lower */, const t_tuple& /* upper */, context& /* h */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> lowerUpperRange_00(const t_tuple& /* lower */, const t_tuple& /* upper */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<t_ind_0::iterator> lowerUpperRange_11(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp == 0) {
    auto pos = ind_0.find(lower, h.hints_0_lower);
    auto fin = ind_0.end();
    if (pos != fin) {fin = pos; ++fin;}
    return make_range(pos, fin);
}
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_11(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_11(lower,upper,h);
}
range<t_ind_0::iterator> lowerUpperRange_01(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_01(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_01(lower,upper,h);
}
bool empty() const {
return ind_0.empty();
}
std::vector<range<iterator>> partition() const {
return ind_0.getChunks(400);
}
void purge() {
ind_0.clear();
}
iterator begin() const {
return ind_0.begin();
}
iterator end() const {
return ind_0.end();
}
void printStatistics(std::ostream& o) const {
o << " arity 2 direct b-tree index 0 lex-order [1,0]\n";
ind_0.printStats(o);
}
};
struct t_btree_iiiiiiiiiiii__0_1_2_3_4_5_6_7_8_9_10_11__111111111111 {
using t_tuple = Tuple<RamDomain, 12>;
Table<t_tuple> dataTable;
Lock insert_lock;
struct t_comparator_0{
 int operator()(const t_tuple *a, const t_tuple *b) const {
  return (ramBitCast<RamSigned>((*a)[0]) <ramBitCast<RamSigned> ((*b)[0])) ? -1 : ((ramBitCast<RamSigned>((*a)[0]) > ramBitCast<RamSigned>((*b)[0])) ? 1 :((ramBitCast<RamSigned>((*a)[1]) <ramBitCast<RamSigned> ((*b)[1])) ? -1 : ((ramBitCast<RamSigned>((*a)[1]) > ramBitCast<RamSigned>((*b)[1])) ? 1 :((ramBitCast<RamSigned>((*a)[2]) <ramBitCast<RamSigned> ((*b)[2])) ? -1 : ((ramBitCast<RamSigned>((*a)[2]) > ramBitCast<RamSigned>((*b)[2])) ? 1 :((ramBitCast<RamSigned>((*a)[3]) <ramBitCast<RamSigned> ((*b)[3])) ? -1 : ((ramBitCast<RamSigned>((*a)[3]) > ramBitCast<RamSigned>((*b)[3])) ? 1 :((ramBitCast<RamSigned>((*a)[4]) <ramBitCast<RamSigned> ((*b)[4])) ? -1 : ((ramBitCast<RamSigned>((*a)[4]) > ramBitCast<RamSigned>((*b)[4])) ? 1 :((ramBitCast<RamSigned>((*a)[5]) <ramBitCast<RamSigned> ((*b)[5])) ? -1 : ((ramBitCast<RamSigned>((*a)[5]) > ramBitCast<RamSigned>((*b)[5])) ? 1 :((ramBitCast<RamSigned>((*a)[6]) <ramBitCast<RamSigned> ((*b)[6])) ? -1 : ((ramBitCast<RamSigned>((*a)[6]) > ramBitCast<RamSigned>((*b)[6])) ? 1 :((ramBitCast<RamSigned>((*a)[7]) <ramBitCast<RamSigned> ((*b)[7])) ? -1 : ((ramBitCast<RamSigned>((*a)[7]) > ramBitCast<RamSigned>((*b)[7])) ? 1 :((ramBitCast<RamSigned>((*a)[8]) <ramBitCast<RamSigned> ((*b)[8])) ? -1 : ((ramBitCast<RamSigned>((*a)[8]) > ramBitCast<RamSigned>((*b)[8])) ? 1 :((ramBitCast<RamSigned>((*a)[9]) <ramBitCast<RamSigned> ((*b)[9])) ? -1 : ((ramBitCast<RamSigned>((*a)[9]) > ramBitCast<RamSigned>((*b)[9])) ? 1 :((ramBitCast<RamSigned>((*a)[10]) <ramBitCast<RamSigned> ((*b)[10])) ? -1 : ((ramBitCast<RamSigned>((*a)[10]) > ramBitCast<RamSigned>((*b)[10])) ? 1 :((ramBitCast<RamSigned>((*a)[11]) <ramBitCast<RamSigned> ((*b)[11])) ? -1 : ((ramBitCast<RamSigned>((*a)[11]) > ramBitCast<RamSigned>((*b)[11])) ? 1 :(0))))))))))))))))))))))));
 }
bool less(const t_tuple *a, const t_tuple *b) const {
  return ramBitCast<RamSigned> ((*a)[0]) < ramBitCast<RamSigned>((*b)[0])|| (ramBitCast<RamSigned>((*a)[0]) == ramBitCast<RamSigned>((*b)[0]) && (ramBitCast<RamSigned> ((*a)[1]) < ramBitCast<RamSigned>((*b)[1])|| (ramBitCast<RamSigned>((*a)[1]) == ramBitCast<RamSigned>((*b)[1]) && (ramBitCast<RamSigned> ((*a)[2]) < ramBitCast<RamSigned>((*b)[2])|| (ramBitCast<RamSigned>((*a)[2]) == ramBitCast<RamSigned>((*b)[2]) && (ramBitCast<RamSigned> ((*a)[3]) < ramBitCast<RamSigned>((*b)[3])|| (ramBitCast<RamSigned>((*a)[3]) == ramBitCast<RamSigned>((*b)[3]) && (ramBitCast<RamSigned> ((*a)[4]) < ramBitCast<RamSigned>((*b)[4])|| (ramBitCast<RamSigned>((*a)[4]) == ramBitCast<RamSigned>((*b)[4]) && (ramBitCast<RamSigned> ((*a)[5]) < ramBitCast<RamSigned>((*b)[5])|| (ramBitCast<RamSigned>((*a)[5]) == ramBitCast<RamSigned>((*b)[5]) && (ramBitCast<RamSigned> ((*a)[6]) < ramBitCast<RamSigned>((*b)[6])|| (ramBitCast<RamSigned>((*a)[6]) == ramBitCast<RamSigned>((*b)[6]) && (ramBitCast<RamSigned> ((*a)[7]) < ramBitCast<RamSigned>((*b)[7])|| (ramBitCast<RamSigned>((*a)[7]) == ramBitCast<RamSigned>((*b)[7]) && (ramBitCast<RamSigned> ((*a)[8]) < ramBitCast<RamSigned>((*b)[8])|| (ramBitCast<RamSigned>((*a)[8]) == ramBitCast<RamSigned>((*b)[8]) && (ramBitCast<RamSigned> ((*a)[9]) < ramBitCast<RamSigned>((*b)[9])|| (ramBitCast<RamSigned>((*a)[9]) == ramBitCast<RamSigned>((*b)[9]) && (ramBitCast<RamSigned> ((*a)[10]) < ramBitCast<RamSigned>((*b)[10])|| (ramBitCast<RamSigned>((*a)[10]) == ramBitCast<RamSigned>((*b)[10]) && (ramBitCast<RamSigned> ((*a)[11]) < ramBitCast<RamSigned>((*b)[11])))))))))))))))))))))));
 }
bool equal(const t_tuple *a, const t_tuple *b) const {
return ramBitCast<RamSigned>((*a)[0]) == ramBitCast<RamSigned>((*b)[0])&&ramBitCast<RamSigned>((*a)[1]) == ramBitCast<RamSigned>((*b)[1])&&ramBitCast<RamSigned>((*a)[2]) == ramBitCast<RamSigned>((*b)[2])&&ramBitCast<RamSigned>((*a)[3]) == ramBitCast<RamSigned>((*b)[3])&&ramBitCast<RamSigned>((*a)[4]) == ramBitCast<RamSigned>((*b)[4])&&ramBitCast<RamSigned>((*a)[5]) == ramBitCast<RamSigned>((*b)[5])&&ramBitCast<RamSigned>((*a)[6]) == ramBitCast<RamSigned>((*b)[6])&&ramBitCast<RamSigned>((*a)[7]) == ramBitCast<RamSigned>((*b)[7])&&ramBitCast<RamSigned>((*a)[8]) == ramBitCast<RamSigned>((*b)[8])&&ramBitCast<RamSigned>((*a)[9]) == ramBitCast<RamSigned>((*b)[9])&&ramBitCast<RamSigned>((*a)[10]) == ramBitCast<RamSigned>((*b)[10])&&ramBitCast<RamSigned>((*a)[11]) == ramBitCast<RamSigned>((*b)[11]);
 }
};
using t_ind_0 = btree_set<const t_tuple*,t_comparator_0>;
t_ind_0 ind_0;
using iterator_0 = IterDerefWrapper<typename t_ind_0::iterator>;
using iterator = iterator_0;
struct context {
t_ind_0::operation_hints hints_0_lower;
t_ind_0::operation_hints hints_0_upper;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
const t_tuple* masterCopy = nullptr;
{
auto lease = insert_lock.acquire();
if (contains(t, h)) return false;
masterCopy = &dataTable.insert(t);
ind_0.insert(masterCopy, h.hints_0_lower);
}
return true;
}
bool insert(const RamDomain* ramDomain) {
RamDomain data[12];
std::copy(ramDomain, ramDomain + 12, data);
const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
context h;
return insert(tuple, h);
}
bool insert(RamDomain a0,RamDomain a1,RamDomain a2,RamDomain a3,RamDomain a4,RamDomain a5,RamDomain a6,RamDomain a7,RamDomain a8,RamDomain a9,RamDomain a10,RamDomain a11) {
RamDomain data[12] = {a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11};
return insert(data);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(&t, h.hints_0_lower);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(&t, h.hints_0_lower);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> lowerUpperRange_0(const t_tuple& lower, const t_tuple& upper, context& h) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> lowerUpperRange_0(const t_tuple& lower, const t_tuple& upper) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator_0> lowerUpperRange_111111111111(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(&lower, &upper);
if (cmp == 0) {
    auto pos = find(lower, h);
    auto fin = end();
    if (pos != fin) {fin = pos; ++fin;}
    return make_range(pos, fin);
}
if (cmp > 0) {
    return range<iterator_0>(ind_0.end(), ind_0.end());
}
return range<iterator_0>(ind_0.lower_bound(&lower, h.hints_0_lower), ind_0.upper_bound(&upper, h.hints_0_upper));
}
range<iterator_0> lowerUpperRange_111111111111(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_111111111111(lower, upper, h);
}
bool empty() const {
return ind_0.empty();
}
std::vector<range<iterator>> partition() const {
std::vector<range<iterator>> res;
for (const auto& cur : ind_0.getChunks(400)) {
    res.push_back(make_range(derefIter(cur.begin()), derefIter(cur.end())));
}
return res;
}
void purge() {
ind_0.clear();
dataTable.clear();
}
iterator begin() const {
return ind_0.begin();
}
iterator end() const {
return ind_0.end();
}
void printStatistics(std::ostream& o) const {
o << " arity 12 indirect b-tree index 0 lex-order [0,1,2,3,4,5,6,7,8,9,10,11]\n";
ind_0.printStats(o);
}
};
struct t_btree_iiiiiiiiiiiii__0_1_2_3_4_5_6_7_8_9_10_11_12__1111111111111 {
using t_tuple = Tuple<RamDomain, 13>;
Table<t_tuple> dataTable;
Lock insert_lock;
struct t_comparator_0{
 int operator()(const t_tuple *a, const t_tuple *b) const {
  return (ramBitCast<RamSigned>((*a)[0]) <ramBitCast<RamSigned> ((*b)[0])) ? -1 : ((ramBitCast<RamSigned>((*a)[0]) > ramBitCast<RamSigned>((*b)[0])) ? 1 :((ramBitCast<RamSigned>((*a)[1]) <ramBitCast<RamSigned> ((*b)[1])) ? -1 : ((ramBitCast<RamSigned>((*a)[1]) > ramBitCast<RamSigned>((*b)[1])) ? 1 :((ramBitCast<RamSigned>((*a)[2]) <ramBitCast<RamSigned> ((*b)[2])) ? -1 : ((ramBitCast<RamSigned>((*a)[2]) > ramBitCast<RamSigned>((*b)[2])) ? 1 :((ramBitCast<RamSigned>((*a)[3]) <ramBitCast<RamSigned> ((*b)[3])) ? -1 : ((ramBitCast<RamSigned>((*a)[3]) > ramBitCast<RamSigned>((*b)[3])) ? 1 :((ramBitCast<RamSigned>((*a)[4]) <ramBitCast<RamSigned> ((*b)[4])) ? -1 : ((ramBitCast<RamSigned>((*a)[4]) > ramBitCast<RamSigned>((*b)[4])) ? 1 :((ramBitCast<RamSigned>((*a)[5]) <ramBitCast<RamSigned> ((*b)[5])) ? -1 : ((ramBitCast<RamSigned>((*a)[5]) > ramBitCast<RamSigned>((*b)[5])) ? 1 :((ramBitCast<RamSigned>((*a)[6]) <ramBitCast<RamSigned> ((*b)[6])) ? -1 : ((ramBitCast<RamSigned>((*a)[6]) > ramBitCast<RamSigned>((*b)[6])) ? 1 :((ramBitCast<RamSigned>((*a)[7]) <ramBitCast<RamSigned> ((*b)[7])) ? -1 : ((ramBitCast<RamSigned>((*a)[7]) > ramBitCast<RamSigned>((*b)[7])) ? 1 :((ramBitCast<RamSigned>((*a)[8]) <ramBitCast<RamSigned> ((*b)[8])) ? -1 : ((ramBitCast<RamSigned>((*a)[8]) > ramBitCast<RamSigned>((*b)[8])) ? 1 :((ramBitCast<RamSigned>((*a)[9]) <ramBitCast<RamSigned> ((*b)[9])) ? -1 : ((ramBitCast<RamSigned>((*a)[9]) > ramBitCast<RamSigned>((*b)[9])) ? 1 :((ramBitCast<RamSigned>((*a)[10]) <ramBitCast<RamSigned> ((*b)[10])) ? -1 : ((ramBitCast<RamSigned>((*a)[10]) > ramBitCast<RamSigned>((*b)[10])) ? 1 :((ramBitCast<RamSigned>((*a)[11]) <ramBitCast<RamSigned> ((*b)[11])) ? -1 : ((ramBitCast<RamSigned>((*a)[11]) > ramBitCast<RamSigned>((*b)[11])) ? 1 :((ramBitCast<RamSigned>((*a)[12]) <ramBitCast<RamSigned> ((*b)[12])) ? -1 : ((ramBitCast<RamSigned>((*a)[12]) > ramBitCast<RamSigned>((*b)[12])) ? 1 :(0))))))))))))))))))))))))));
 }
bool less(const t_tuple *a, const t_tuple *b) const {
  return ramBitCast<RamSigned> ((*a)[0]) < ramBitCast<RamSigned>((*b)[0])|| (ramBitCast<RamSigned>((*a)[0]) == ramBitCast<RamSigned>((*b)[0]) && (ramBitCast<RamSigned> ((*a)[1]) < ramBitCast<RamSigned>((*b)[1])|| (ramBitCast<RamSigned>((*a)[1]) == ramBitCast<RamSigned>((*b)[1]) && (ramBitCast<RamSigned> ((*a)[2]) < ramBitCast<RamSigned>((*b)[2])|| (ramBitCast<RamSigned>((*a)[2]) == ramBitCast<RamSigned>((*b)[2]) && (ramBitCast<RamSigned> ((*a)[3]) < ramBitCast<RamSigned>((*b)[3])|| (ramBitCast<RamSigned>((*a)[3]) == ramBitCast<RamSigned>((*b)[3]) && (ramBitCast<RamSigned> ((*a)[4]) < ramBitCast<RamSigned>((*b)[4])|| (ramBitCast<RamSigned>((*a)[4]) == ramBitCast<RamSigned>((*b)[4]) && (ramBitCast<RamSigned> ((*a)[5]) < ramBitCast<RamSigned>((*b)[5])|| (ramBitCast<RamSigned>((*a)[5]) == ramBitCast<RamSigned>((*b)[5]) && (ramBitCast<RamSigned> ((*a)[6]) < ramBitCast<RamSigned>((*b)[6])|| (ramBitCast<RamSigned>((*a)[6]) == ramBitCast<RamSigned>((*b)[6]) && (ramBitCast<RamSigned> ((*a)[7]) < ramBitCast<RamSigned>((*b)[7])|| (ramBitCast<RamSigned>((*a)[7]) == ramBitCast<RamSigned>((*b)[7]) && (ramBitCast<RamSigned> ((*a)[8]) < ramBitCast<RamSigned>((*b)[8])|| (ramBitCast<RamSigned>((*a)[8]) == ramBitCast<RamSigned>((*b)[8]) && (ramBitCast<RamSigned> ((*a)[9]) < ramBitCast<RamSigned>((*b)[9])|| (ramBitCast<RamSigned>((*a)[9]) == ramBitCast<RamSigned>((*b)[9]) && (ramBitCast<RamSigned> ((*a)[10]) < ramBitCast<RamSigned>((*b)[10])|| (ramBitCast<RamSigned>((*a)[10]) == ramBitCast<RamSigned>((*b)[10]) && (ramBitCast<RamSigned> ((*a)[11]) < ramBitCast<RamSigned>((*b)[11])|| (ramBitCast<RamSigned>((*a)[11]) == ramBitCast<RamSigned>((*b)[11]) && (ramBitCast<RamSigned> ((*a)[12]) < ramBitCast<RamSigned>((*b)[12])))))))))))))))))))))))));
 }
bool equal(const t_tuple *a, const t_tuple *b) const {
return ramBitCast<RamSigned>((*a)[0]) == ramBitCast<RamSigned>((*b)[0])&&ramBitCast<RamSigned>((*a)[1]) == ramBitCast<RamSigned>((*b)[1])&&ramBitCast<RamSigned>((*a)[2]) == ramBitCast<RamSigned>((*b)[2])&&ramBitCast<RamSigned>((*a)[3]) == ramBitCast<RamSigned>((*b)[3])&&ramBitCast<RamSigned>((*a)[4]) == ramBitCast<RamSigned>((*b)[4])&&ramBitCast<RamSigned>((*a)[5]) == ramBitCast<RamSigned>((*b)[5])&&ramBitCast<RamSigned>((*a)[6]) == ramBitCast<RamSigned>((*b)[6])&&ramBitCast<RamSigned>((*a)[7]) == ramBitCast<RamSigned>((*b)[7])&&ramBitCast<RamSigned>((*a)[8]) == ramBitCast<RamSigned>((*b)[8])&&ramBitCast<RamSigned>((*a)[9]) == ramBitCast<RamSigned>((*b)[9])&&ramBitCast<RamSigned>((*a)[10]) == ramBitCast<RamSigned>((*b)[10])&&ramBitCast<RamSigned>((*a)[11]) == ramBitCast<RamSigned>((*b)[11])&&ramBitCast<RamSigned>((*a)[12]) == ramBitCast<RamSigned>((*b)[12]);
 }
};
using t_ind_0 = btree_set<const t_tuple*,t_comparator_0>;
t_ind_0 ind_0;
using iterator_0 = IterDerefWrapper<typename t_ind_0::iterator>;
using iterator = iterator_0;
struct context {
t_ind_0::operation_hints hints_0_lower;
t_ind_0::operation_hints hints_0_upper;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
const t_tuple* masterCopy = nullptr;
{
auto lease = insert_lock.acquire();
if (contains(t, h)) return false;
masterCopy = &dataTable.insert(t);
ind_0.insert(masterCopy, h.hints_0_lower);
}
return true;
}
bool insert(const RamDomain* ramDomain) {
RamDomain data[13];
std::copy(ramDomain, ramDomain + 13, data);
const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
context h;
return insert(tuple, h);
}
bool insert(RamDomain a0,RamDomain a1,RamDomain a2,RamDomain a3,RamDomain a4,RamDomain a5,RamDomain a6,RamDomain a7,RamDomain a8,RamDomain a9,RamDomain a10,RamDomain a11,RamDomain a12) {
RamDomain data[13] = {a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12};
return insert(data);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(&t, h.hints_0_lower);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(&t, h.hints_0_lower);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> lowerUpperRange_0(const t_tuple& lower, const t_tuple& upper, context& h) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> lowerUpperRange_0(const t_tuple& lower, const t_tuple& upper) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator_0> lowerUpperRange_1111111111111(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(&lower, &upper);
if (cmp == 0) {
    auto pos = find(lower, h);
    auto fin = end();
    if (pos != fin) {fin = pos; ++fin;}
    return make_range(pos, fin);
}
if (cmp > 0) {
    return range<iterator_0>(ind_0.end(), ind_0.end());
}
return range<iterator_0>(ind_0.lower_bound(&lower, h.hints_0_lower), ind_0.upper_bound(&upper, h.hints_0_upper));
}
range<iterator_0> lowerUpperRange_1111111111111(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_1111111111111(lower, upper, h);
}
bool empty() const {
return ind_0.empty();
}
std::vector<range<iterator>> partition() const {
std::vector<range<iterator>> res;
for (const auto& cur : ind_0.getChunks(400)) {
    res.push_back(make_range(derefIter(cur.begin()), derefIter(cur.end())));
}
return res;
}
void purge() {
ind_0.clear();
dataTable.clear();
}
iterator begin() const {
return ind_0.begin();
}
iterator end() const {
return ind_0.end();
}
void printStatistics(std::ostream& o) const {
o << " arity 13 indirect b-tree index 0 lex-order [0,1,2,3,4,5,6,7,8,9,10,11,12]\n";
ind_0.printStats(o);
}
};
struct t_btree_iiiiiiiii__1_0_2_3_4_5_6_7_8__010000000__111111111 {
using t_tuple = Tuple<RamDomain, 9>;
Table<t_tuple> dataTable;
Lock insert_lock;
struct t_comparator_0{
 int operator()(const t_tuple *a, const t_tuple *b) const {
  return (ramBitCast<RamSigned>((*a)[1]) <ramBitCast<RamSigned> ((*b)[1])) ? -1 : ((ramBitCast<RamSigned>((*a)[1]) > ramBitCast<RamSigned>((*b)[1])) ? 1 :((ramBitCast<RamSigned>((*a)[0]) <ramBitCast<RamSigned> ((*b)[0])) ? -1 : ((ramBitCast<RamSigned>((*a)[0]) > ramBitCast<RamSigned>((*b)[0])) ? 1 :((ramBitCast<RamSigned>((*a)[2]) <ramBitCast<RamSigned> ((*b)[2])) ? -1 : ((ramBitCast<RamSigned>((*a)[2]) > ramBitCast<RamSigned>((*b)[2])) ? 1 :((ramBitCast<RamSigned>((*a)[3]) <ramBitCast<RamSigned> ((*b)[3])) ? -1 : ((ramBitCast<RamSigned>((*a)[3]) > ramBitCast<RamSigned>((*b)[3])) ? 1 :((ramBitCast<RamSigned>((*a)[4]) <ramBitCast<RamSigned> ((*b)[4])) ? -1 : ((ramBitCast<RamSigned>((*a)[4]) > ramBitCast<RamSigned>((*b)[4])) ? 1 :((ramBitCast<RamSigned>((*a)[5]) <ramBitCast<RamSigned> ((*b)[5])) ? -1 : ((ramBitCast<RamSigned>((*a)[5]) > ramBitCast<RamSigned>((*b)[5])) ? 1 :((ramBitCast<RamSigned>((*a)[6]) <ramBitCast<RamSigned> ((*b)[6])) ? -1 : ((ramBitCast<RamSigned>((*a)[6]) > ramBitCast<RamSigned>((*b)[6])) ? 1 :((ramBitCast<RamSigned>((*a)[7]) <ramBitCast<RamSigned> ((*b)[7])) ? -1 : ((ramBitCast<RamSigned>((*a)[7]) > ramBitCast<RamSigned>((*b)[7])) ? 1 :((ramBitCast<RamSigned>((*a)[8]) <ramBitCast<RamSigned> ((*b)[8])) ? -1 : ((ramBitCast<RamSigned>((*a)[8]) > ramBitCast<RamSigned>((*b)[8])) ? 1 :(0))))))))))))))))));
 }
bool less(const t_tuple *a, const t_tuple *b) const {
  return ramBitCast<RamSigned> ((*a)[1]) < ramBitCast<RamSigned>((*b)[1])|| (ramBitCast<RamSigned>((*a)[1]) == ramBitCast<RamSigned>((*b)[1]) && (ramBitCast<RamSigned> ((*a)[0]) < ramBitCast<RamSigned>((*b)[0])|| (ramBitCast<RamSigned>((*a)[0]) == ramBitCast<RamSigned>((*b)[0]) && (ramBitCast<RamSigned> ((*a)[2]) < ramBitCast<RamSigned>((*b)[2])|| (ramBitCast<RamSigned>((*a)[2]) == ramBitCast<RamSigned>((*b)[2]) && (ramBitCast<RamSigned> ((*a)[3]) < ramBitCast<RamSigned>((*b)[3])|| (ramBitCast<RamSigned>((*a)[3]) == ramBitCast<RamSigned>((*b)[3]) && (ramBitCast<RamSigned> ((*a)[4]) < ramBitCast<RamSigned>((*b)[4])|| (ramBitCast<RamSigned>((*a)[4]) == ramBitCast<RamSigned>((*b)[4]) && (ramBitCast<RamSigned> ((*a)[5]) < ramBitCast<RamSigned>((*b)[5])|| (ramBitCast<RamSigned>((*a)[5]) == ramBitCast<RamSigned>((*b)[5]) && (ramBitCast<RamSigned> ((*a)[6]) < ramBitCast<RamSigned>((*b)[6])|| (ramBitCast<RamSigned>((*a)[6]) == ramBitCast<RamSigned>((*b)[6]) && (ramBitCast<RamSigned> ((*a)[7]) < ramBitCast<RamSigned>((*b)[7])|| (ramBitCast<RamSigned>((*a)[7]) == ramBitCast<RamSigned>((*b)[7]) && (ramBitCast<RamSigned> ((*a)[8]) < ramBitCast<RamSigned>((*b)[8])))))))))))))))));
 }
bool equal(const t_tuple *a, const t_tuple *b) const {
return ramBitCast<RamSigned>((*a)[1]) == ramBitCast<RamSigned>((*b)[1])&&ramBitCast<RamSigned>((*a)[0]) == ramBitCast<RamSigned>((*b)[0])&&ramBitCast<RamSigned>((*a)[2]) == ramBitCast<RamSigned>((*b)[2])&&ramBitCast<RamSigned>((*a)[3]) == ramBitCast<RamSigned>((*b)[3])&&ramBitCast<RamSigned>((*a)[4]) == ramBitCast<RamSigned>((*b)[4])&&ramBitCast<RamSigned>((*a)[5]) == ramBitCast<RamSigned>((*b)[5])&&ramBitCast<RamSigned>((*a)[6]) == ramBitCast<RamSigned>((*b)[6])&&ramBitCast<RamSigned>((*a)[7]) == ramBitCast<RamSigned>((*b)[7])&&ramBitCast<RamSigned>((*a)[8]) == ramBitCast<RamSigned>((*b)[8]);
 }
};
using t_ind_0 = btree_set<const t_tuple*,t_comparator_0>;
t_ind_0 ind_0;
using iterator_0 = IterDerefWrapper<typename t_ind_0::iterator>;
using iterator = iterator_0;
struct context {
t_ind_0::operation_hints hints_0_lower;
t_ind_0::operation_hints hints_0_upper;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
const t_tuple* masterCopy = nullptr;
{
auto lease = insert_lock.acquire();
if (contains(t, h)) return false;
masterCopy = &dataTable.insert(t);
ind_0.insert(masterCopy, h.hints_0_lower);
}
return true;
}
bool insert(const RamDomain* ramDomain) {
RamDomain data[9];
std::copy(ramDomain, ramDomain + 9, data);
const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
context h;
return insert(tuple, h);
}
bool insert(RamDomain a0,RamDomain a1,RamDomain a2,RamDomain a3,RamDomain a4,RamDomain a5,RamDomain a6,RamDomain a7,RamDomain a8) {
RamDomain data[9] = {a0,a1,a2,a3,a4,a5,a6,a7,a8};
return insert(data);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(&t, h.hints_0_lower);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(&t, h.hints_0_lower);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> lowerUpperRange_0(const t_tuple& lower, const t_tuple& upper, context& h) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> lowerUpperRange_0(const t_tuple& lower, const t_tuple& upper) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator_0> lowerUpperRange_010000000(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(&lower, &upper);
if (cmp > 0) {
    return range<iterator_0>(ind_0.end(), ind_0.end());
}
return range<iterator_0>(ind_0.lower_bound(&lower, h.hints_0_lower), ind_0.upper_bound(&upper, h.hints_0_upper));
}
range<iterator_0> lowerUpperRange_010000000(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_010000000(lower, upper, h);
}
range<iterator_0> lowerUpperRange_111111111(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(&lower, &upper);
if (cmp == 0) {
    auto pos = find(lower, h);
    auto fin = end();
    if (pos != fin) {fin = pos; ++fin;}
    return make_range(pos, fin);
}
if (cmp > 0) {
    return range<iterator_0>(ind_0.end(), ind_0.end());
}
return range<iterator_0>(ind_0.lower_bound(&lower, h.hints_0_lower), ind_0.upper_bound(&upper, h.hints_0_upper));
}
range<iterator_0> lowerUpperRange_111111111(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_111111111(lower, upper, h);
}
bool empty() const {
return ind_0.empty();
}
std::vector<range<iterator>> partition() const {
std::vector<range<iterator>> res;
for (const auto& cur : ind_0.getChunks(400)) {
    res.push_back(make_range(derefIter(cur.begin()), derefIter(cur.end())));
}
return res;
}
void purge() {
ind_0.clear();
dataTable.clear();
}
iterator begin() const {
return ind_0.begin();
}
iterator end() const {
return ind_0.end();
}
void printStatistics(std::ostream& o) const {
o << " arity 9 indirect b-tree index 0 lex-order [1,0,2,3,4,5,6,7,8]\n";
ind_0.printStats(o);
}
};
struct t_btree_iiiiiiiiiiiiiiii__0_1_2_3_4_5_6_7_8_9_10_11_12_13_14_15__1111111111111111 {
using t_tuple = Tuple<RamDomain, 16>;
Table<t_tuple> dataTable;
Lock insert_lock;
struct t_comparator_0{
 int operator()(const t_tuple *a, const t_tuple *b) const {
  return (ramBitCast<RamSigned>((*a)[0]) <ramBitCast<RamSigned> ((*b)[0])) ? -1 : ((ramBitCast<RamSigned>((*a)[0]) > ramBitCast<RamSigned>((*b)[0])) ? 1 :((ramBitCast<RamSigned>((*a)[1]) <ramBitCast<RamSigned> ((*b)[1])) ? -1 : ((ramBitCast<RamSigned>((*a)[1]) > ramBitCast<RamSigned>((*b)[1])) ? 1 :((ramBitCast<RamSigned>((*a)[2]) <ramBitCast<RamSigned> ((*b)[2])) ? -1 : ((ramBitCast<RamSigned>((*a)[2]) > ramBitCast<RamSigned>((*b)[2])) ? 1 :((ramBitCast<RamSigned>((*a)[3]) <ramBitCast<RamSigned> ((*b)[3])) ? -1 : ((ramBitCast<RamSigned>((*a)[3]) > ramBitCast<RamSigned>((*b)[3])) ? 1 :((ramBitCast<RamSigned>((*a)[4]) <ramBitCast<RamSigned> ((*b)[4])) ? -1 : ((ramBitCast<RamSigned>((*a)[4]) > ramBitCast<RamSigned>((*b)[4])) ? 1 :((ramBitCast<RamSigned>((*a)[5]) <ramBitCast<RamSigned> ((*b)[5])) ? -1 : ((ramBitCast<RamSigned>((*a)[5]) > ramBitCast<RamSigned>((*b)[5])) ? 1 :((ramBitCast<RamSigned>((*a)[6]) <ramBitCast<RamSigned> ((*b)[6])) ? -1 : ((ramBitCast<RamSigned>((*a)[6]) > ramBitCast<RamSigned>((*b)[6])) ? 1 :((ramBitCast<RamSigned>((*a)[7]) <ramBitCast<RamSigned> ((*b)[7])) ? -1 : ((ramBitCast<RamSigned>((*a)[7]) > ramBitCast<RamSigned>((*b)[7])) ? 1 :((ramBitCast<RamSigned>((*a)[8]) <ramBitCast<RamSigned> ((*b)[8])) ? -1 : ((ramBitCast<RamSigned>((*a)[8]) > ramBitCast<RamSigned>((*b)[8])) ? 1 :((ramBitCast<RamSigned>((*a)[9]) <ramBitCast<RamSigned> ((*b)[9])) ? -1 : ((ramBitCast<RamSigned>((*a)[9]) > ramBitCast<RamSigned>((*b)[9])) ? 1 :((ramBitCast<RamSigned>((*a)[10]) <ramBitCast<RamSigned> ((*b)[10])) ? -1 : ((ramBitCast<RamSigned>((*a)[10]) > ramBitCast<RamSigned>((*b)[10])) ? 1 :((ramBitCast<RamSigned>((*a)[11]) <ramBitCast<RamSigned> ((*b)[11])) ? -1 : ((ramBitCast<RamSigned>((*a)[11]) > ramBitCast<RamSigned>((*b)[11])) ? 1 :((ramBitCast<RamSigned>((*a)[12]) <ramBitCast<RamSigned> ((*b)[12])) ? -1 : ((ramBitCast<RamSigned>((*a)[12]) > ramBitCast<RamSigned>((*b)[12])) ? 1 :((ramBitCast<RamSigned>((*a)[13]) <ramBitCast<RamSigned> ((*b)[13])) ? -1 : ((ramBitCast<RamSigned>((*a)[13]) > ramBitCast<RamSigned>((*b)[13])) ? 1 :((ramBitCast<RamSigned>((*a)[14]) <ramBitCast<RamSigned> ((*b)[14])) ? -1 : ((ramBitCast<RamSigned>((*a)[14]) > ramBitCast<RamSigned>((*b)[14])) ? 1 :((ramBitCast<RamSigned>((*a)[15]) <ramBitCast<RamSigned> ((*b)[15])) ? -1 : ((ramBitCast<RamSigned>((*a)[15]) > ramBitCast<RamSigned>((*b)[15])) ? 1 :(0))))))))))))))))))))))))))))))));
 }
bool less(const t_tuple *a, const t_tuple *b) const {
  return ramBitCast<RamSigned> ((*a)[0]) < ramBitCast<RamSigned>((*b)[0])|| (ramBitCast<RamSigned>((*a)[0]) == ramBitCast<RamSigned>((*b)[0]) && (ramBitCast<RamSigned> ((*a)[1]) < ramBitCast<RamSigned>((*b)[1])|| (ramBitCast<RamSigned>((*a)[1]) == ramBitCast<RamSigned>((*b)[1]) && (ramBitCast<RamSigned> ((*a)[2]) < ramBitCast<RamSigned>((*b)[2])|| (ramBitCast<RamSigned>((*a)[2]) == ramBitCast<RamSigned>((*b)[2]) && (ramBitCast<RamSigned> ((*a)[3]) < ramBitCast<RamSigned>((*b)[3])|| (ramBitCast<RamSigned>((*a)[3]) == ramBitCast<RamSigned>((*b)[3]) && (ramBitCast<RamSigned> ((*a)[4]) < ramBitCast<RamSigned>((*b)[4])|| (ramBitCast<RamSigned>((*a)[4]) == ramBitCast<RamSigned>((*b)[4]) && (ramBitCast<RamSigned> ((*a)[5]) < ramBitCast<RamSigned>((*b)[5])|| (ramBitCast<RamSigned>((*a)[5]) == ramBitCast<RamSigned>((*b)[5]) && (ramBitCast<RamSigned> ((*a)[6]) < ramBitCast<RamSigned>((*b)[6])|| (ramBitCast<RamSigned>((*a)[6]) == ramBitCast<RamSigned>((*b)[6]) && (ramBitCast<RamSigned> ((*a)[7]) < ramBitCast<RamSigned>((*b)[7])|| (ramBitCast<RamSigned>((*a)[7]) == ramBitCast<RamSigned>((*b)[7]) && (ramBitCast<RamSigned> ((*a)[8]) < ramBitCast<RamSigned>((*b)[8])|| (ramBitCast<RamSigned>((*a)[8]) == ramBitCast<RamSigned>((*b)[8]) && (ramBitCast<RamSigned> ((*a)[9]) < ramBitCast<RamSigned>((*b)[9])|| (ramBitCast<RamSigned>((*a)[9]) == ramBitCast<RamSigned>((*b)[9]) && (ramBitCast<RamSigned> ((*a)[10]) < ramBitCast<RamSigned>((*b)[10])|| (ramBitCast<RamSigned>((*a)[10]) == ramBitCast<RamSigned>((*b)[10]) && (ramBitCast<RamSigned> ((*a)[11]) < ramBitCast<RamSigned>((*b)[11])|| (ramBitCast<RamSigned>((*a)[11]) == ramBitCast<RamSigned>((*b)[11]) && (ramBitCast<RamSigned> ((*a)[12]) < ramBitCast<RamSigned>((*b)[12])|| (ramBitCast<RamSigned>((*a)[12]) == ramBitCast<RamSigned>((*b)[12]) && (ramBitCast<RamSigned> ((*a)[13]) < ramBitCast<RamSigned>((*b)[13])|| (ramBitCast<RamSigned>((*a)[13]) == ramBitCast<RamSigned>((*b)[13]) && (ramBitCast<RamSigned> ((*a)[14]) < ramBitCast<RamSigned>((*b)[14])|| (ramBitCast<RamSigned>((*a)[14]) == ramBitCast<RamSigned>((*b)[14]) && (ramBitCast<RamSigned> ((*a)[15]) < ramBitCast<RamSigned>((*b)[15])))))))))))))))))))))))))))))));
 }
bool equal(const t_tuple *a, const t_tuple *b) const {
return ramBitCast<RamSigned>((*a)[0]) == ramBitCast<RamSigned>((*b)[0])&&ramBitCast<RamSigned>((*a)[1]) == ramBitCast<RamSigned>((*b)[1])&&ramBitCast<RamSigned>((*a)[2]) == ramBitCast<RamSigned>((*b)[2])&&ramBitCast<RamSigned>((*a)[3]) == ramBitCast<RamSigned>((*b)[3])&&ramBitCast<RamSigned>((*a)[4]) == ramBitCast<RamSigned>((*b)[4])&&ramBitCast<RamSigned>((*a)[5]) == ramBitCast<RamSigned>((*b)[5])&&ramBitCast<RamSigned>((*a)[6]) == ramBitCast<RamSigned>((*b)[6])&&ramBitCast<RamSigned>((*a)[7]) == ramBitCast<RamSigned>((*b)[7])&&ramBitCast<RamSigned>((*a)[8]) == ramBitCast<RamSigned>((*b)[8])&&ramBitCast<RamSigned>((*a)[9]) == ramBitCast<RamSigned>((*b)[9])&&ramBitCast<RamSigned>((*a)[10]) == ramBitCast<RamSigned>((*b)[10])&&ramBitCast<RamSigned>((*a)[11]) == ramBitCast<RamSigned>((*b)[11])&&ramBitCast<RamSigned>((*a)[12]) == ramBitCast<RamSigned>((*b)[12])&&ramBitCast<RamSigned>((*a)[13]) == ramBitCast<RamSigned>((*b)[13])&&ramBitCast<RamSigned>((*a)[14]) == ramBitCast<RamSigned>((*b)[14])&&ramBitCast<RamSigned>((*a)[15]) == ramBitCast<RamSigned>((*b)[15]);
 }
};
using t_ind_0 = btree_set<const t_tuple*,t_comparator_0>;
t_ind_0 ind_0;
using iterator_0 = IterDerefWrapper<typename t_ind_0::iterator>;
using iterator = iterator_0;
struct context {
t_ind_0::operation_hints hints_0_lower;
t_ind_0::operation_hints hints_0_upper;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
const t_tuple* masterCopy = nullptr;
{
auto lease = insert_lock.acquire();
if (contains(t, h)) return false;
masterCopy = &dataTable.insert(t);
ind_0.insert(masterCopy, h.hints_0_lower);
}
return true;
}
bool insert(const RamDomain* ramDomain) {
RamDomain data[16];
std::copy(ramDomain, ramDomain + 16, data);
const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
context h;
return insert(tuple, h);
}
bool insert(RamDomain a0,RamDomain a1,RamDomain a2,RamDomain a3,RamDomain a4,RamDomain a5,RamDomain a6,RamDomain a7,RamDomain a8,RamDomain a9,RamDomain a10,RamDomain a11,RamDomain a12,RamDomain a13,RamDomain a14,RamDomain a15) {
RamDomain data[16] = {a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15};
return insert(data);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(&t, h.hints_0_lower);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(&t, h.hints_0_lower);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> lowerUpperRange_0(const t_tuple& lower, const t_tuple& upper, context& h) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> lowerUpperRange_0(const t_tuple& lower, const t_tuple& upper) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator_0> lowerUpperRange_1111111111111111(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(&lower, &upper);
if (cmp == 0) {
    auto pos = find(lower, h);
    auto fin = end();
    if (pos != fin) {fin = pos; ++fin;}
    return make_range(pos, fin);
}
if (cmp > 0) {
    return range<iterator_0>(ind_0.end(), ind_0.end());
}
return range<iterator_0>(ind_0.lower_bound(&lower, h.hints_0_lower), ind_0.upper_bound(&upper, h.hints_0_upper));
}
range<iterator_0> lowerUpperRange_1111111111111111(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_1111111111111111(lower, upper, h);
}
bool empty() const {
return ind_0.empty();
}
std::vector<range<iterator>> partition() const {
std::vector<range<iterator>> res;
for (const auto& cur : ind_0.getChunks(400)) {
    res.push_back(make_range(derefIter(cur.begin()), derefIter(cur.end())));
}
return res;
}
void purge() {
ind_0.clear();
dataTable.clear();
}
iterator begin() const {
return ind_0.begin();
}
iterator end() const {
return ind_0.end();
}
void printStatistics(std::ostream& o) const {
o << " arity 16 indirect b-tree index 0 lex-order [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]\n";
ind_0.printStats(o);
}
};
struct t_btree_iii__0_1_2__100__110__111 {
using t_tuple = Tuple<RamDomain, 3>;
struct t_comparator_0{
 int operator()(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0])) ? -1 : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0])) ? 1 :((ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1])) ? -1 : (ramBitCast<RamSigned>(a[1]) > ramBitCast<RamSigned>(b[1])) ? 1 :((ramBitCast<RamSigned>(a[2]) < ramBitCast<RamSigned>(b[2])) ? -1 : (ramBitCast<RamSigned>(a[2]) > ramBitCast<RamSigned>(b[2])) ? 1 :(0)));
 }
bool less(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]))|| (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0])) && ((ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1]))|| (ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1])) && ((ramBitCast<RamSigned>(a[2]) < ramBitCast<RamSigned>(b[2]))));
 }
bool equal(const t_tuple& a, const t_tuple& b) const {
return (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0]))&&(ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1]))&&(ramBitCast<RamSigned>(a[2]) == ramBitCast<RamSigned>(b[2]));
 }
};
using t_ind_0 = btree_set<t_tuple,t_comparator_0>;
t_ind_0 ind_0;
using iterator = t_ind_0::iterator;
struct context {
t_ind_0::operation_hints hints_0_lower;
t_ind_0::operation_hints hints_0_upper;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
if (ind_0.insert(t, h.hints_0_lower)) {
return true;
} else return false;
}
bool insert(const RamDomain* ramDomain) {
RamDomain data[3];
std::copy(ramDomain, ramDomain + 3, data);
const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
context h;
return insert(tuple, h);
}
bool insert(RamDomain a0,RamDomain a1,RamDomain a2) {
RamDomain data[3] = {a0,a1,a2};
return insert(data);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(t, h.hints_0_lower);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(t, h.hints_0_lower);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> lowerUpperRange_000(const t_tuple& /* lower */, const t_tuple& /* upper */, context& /* h */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> lowerUpperRange_000(const t_tuple& /* lower */, const t_tuple& /* upper */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<t_ind_0::iterator> lowerUpperRange_100(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_100(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_100(lower,upper,h);
}
range<t_ind_0::iterator> lowerUpperRange_110(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_110(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_110(lower,upper,h);
}
range<t_ind_0::iterator> lowerUpperRange_111(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp == 0) {
    auto pos = ind_0.find(lower, h.hints_0_lower);
    auto fin = ind_0.end();
    if (pos != fin) {fin = pos; ++fin;}
    return make_range(pos, fin);
}
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_111(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_111(lower,upper,h);
}
bool empty() const {
return ind_0.empty();
}
std::vector<range<iterator>> partition() const {
return ind_0.getChunks(400);
}
void purge() {
ind_0.clear();
}
iterator begin() const {
return ind_0.begin();
}
iterator end() const {
return ind_0.end();
}
void printStatistics(std::ostream& o) const {
o << " arity 3 direct b-tree index 0 lex-order [0,1,2]\n";
ind_0.printStats(o);
}
};
struct t_btree_iii__1_0_2__010__111 {
using t_tuple = Tuple<RamDomain, 3>;
struct t_comparator_0{
 int operator()(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1])) ? -1 : (ramBitCast<RamSigned>(a[1]) > ramBitCast<RamSigned>(b[1])) ? 1 :((ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0])) ? -1 : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0])) ? 1 :((ramBitCast<RamSigned>(a[2]) < ramBitCast<RamSigned>(b[2])) ? -1 : (ramBitCast<RamSigned>(a[2]) > ramBitCast<RamSigned>(b[2])) ? 1 :(0)));
 }
bool less(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1]))|| (ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1])) && ((ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]))|| (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0])) && ((ramBitCast<RamSigned>(a[2]) < ramBitCast<RamSigned>(b[2]))));
 }
bool equal(const t_tuple& a, const t_tuple& b) const {
return (ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1]))&&(ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0]))&&(ramBitCast<RamSigned>(a[2]) == ramBitCast<RamSigned>(b[2]));
 }
};
using t_ind_0 = btree_set<t_tuple,t_comparator_0>;
t_ind_0 ind_0;
using iterator = t_ind_0::iterator;
struct context {
t_ind_0::operation_hints hints_0_lower;
t_ind_0::operation_hints hints_0_upper;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
if (ind_0.insert(t, h.hints_0_lower)) {
return true;
} else return false;
}
bool insert(const RamDomain* ramDomain) {
RamDomain data[3];
std::copy(ramDomain, ramDomain + 3, data);
const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
context h;
return insert(tuple, h);
}
bool insert(RamDomain a0,RamDomain a1,RamDomain a2) {
RamDomain data[3] = {a0,a1,a2};
return insert(data);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(t, h.hints_0_lower);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(t, h.hints_0_lower);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> lowerUpperRange_000(const t_tuple& /* lower */, const t_tuple& /* upper */, context& /* h */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> lowerUpperRange_000(const t_tuple& /* lower */, const t_tuple& /* upper */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<t_ind_0::iterator> lowerUpperRange_010(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_010(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_010(lower,upper,h);
}
range<t_ind_0::iterator> lowerUpperRange_111(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp == 0) {
    auto pos = ind_0.find(lower, h.hints_0_lower);
    auto fin = ind_0.end();
    if (pos != fin) {fin = pos; ++fin;}
    return make_range(pos, fin);
}
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_111(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_111(lower,upper,h);
}
bool empty() const {
return ind_0.empty();
}
std::vector<range<iterator>> partition() const {
return ind_0.getChunks(400);
}
void purge() {
ind_0.clear();
}
iterator begin() const {
return ind_0.begin();
}
iterator end() const {
return ind_0.end();
}
void printStatistics(std::ostream& o) const {
o << " arity 3 direct b-tree index 0 lex-order [1,0,2]\n";
ind_0.printStats(o);
}
};
struct t_btree_iiii__1_0_2_3__1111__0100 {
using t_tuple = Tuple<RamDomain, 4>;
struct t_comparator_0{
 int operator()(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1])) ? -1 : (ramBitCast<RamSigned>(a[1]) > ramBitCast<RamSigned>(b[1])) ? 1 :((ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0])) ? -1 : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0])) ? 1 :((ramBitCast<RamSigned>(a[2]) < ramBitCast<RamSigned>(b[2])) ? -1 : (ramBitCast<RamSigned>(a[2]) > ramBitCast<RamSigned>(b[2])) ? 1 :((ramBitCast<RamSigned>(a[3]) < ramBitCast<RamSigned>(b[3])) ? -1 : (ramBitCast<RamSigned>(a[3]) > ramBitCast<RamSigned>(b[3])) ? 1 :(0))));
 }
bool less(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1]))|| (ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1])) && ((ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]))|| (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0])) && ((ramBitCast<RamSigned>(a[2]) < ramBitCast<RamSigned>(b[2]))|| (ramBitCast<RamSigned>(a[2]) == ramBitCast<RamSigned>(b[2])) && ((ramBitCast<RamSigned>(a[3]) < ramBitCast<RamSigned>(b[3])))));
 }
bool equal(const t_tuple& a, const t_tuple& b) const {
return (ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1]))&&(ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0]))&&(ramBitCast<RamSigned>(a[2]) == ramBitCast<RamSigned>(b[2]))&&(ramBitCast<RamSigned>(a[3]) == ramBitCast<RamSigned>(b[3]));
 }
};
using t_ind_0 = btree_set<t_tuple,t_comparator_0>;
t_ind_0 ind_0;
using iterator = t_ind_0::iterator;
struct context {
t_ind_0::operation_hints hints_0_lower;
t_ind_0::operation_hints hints_0_upper;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
if (ind_0.insert(t, h.hints_0_lower)) {
return true;
} else return false;
}
bool insert(const RamDomain* ramDomain) {
RamDomain data[4];
std::copy(ramDomain, ramDomain + 4, data);
const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
context h;
return insert(tuple, h);
}
bool insert(RamDomain a0,RamDomain a1,RamDomain a2,RamDomain a3) {
RamDomain data[4] = {a0,a1,a2,a3};
return insert(data);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(t, h.hints_0_lower);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(t, h.hints_0_lower);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> lowerUpperRange_0000(const t_tuple& /* lower */, const t_tuple& /* upper */, context& /* h */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> lowerUpperRange_0000(const t_tuple& /* lower */, const t_tuple& /* upper */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<t_ind_0::iterator> lowerUpperRange_1111(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp == 0) {
    auto pos = ind_0.find(lower, h.hints_0_lower);
    auto fin = ind_0.end();
    if (pos != fin) {fin = pos; ++fin;}
    return make_range(pos, fin);
}
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_1111(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_1111(lower,upper,h);
}
range<t_ind_0::iterator> lowerUpperRange_0100(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_0100(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_0100(lower,upper,h);
}
bool empty() const {
return ind_0.empty();
}
std::vector<range<iterator>> partition() const {
return ind_0.getChunks(400);
}
void purge() {
ind_0.clear();
}
iterator begin() const {
return ind_0.begin();
}
iterator end() const {
return ind_0.end();
}
void printStatistics(std::ostream& o) const {
o << " arity 4 direct b-tree index 0 lex-order [1,0,2,3]\n";
ind_0.printStats(o);
}
};
struct t_btree_ii__0_1__11 {
using t_tuple = Tuple<RamDomain, 2>;
struct t_comparator_0{
 int operator()(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0])) ? -1 : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0])) ? 1 :((ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1])) ? -1 : (ramBitCast<RamSigned>(a[1]) > ramBitCast<RamSigned>(b[1])) ? 1 :(0));
 }
bool less(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]))|| (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0])) && ((ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1])));
 }
bool equal(const t_tuple& a, const t_tuple& b) const {
return (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0]))&&(ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1]));
 }
};
using t_ind_0 = btree_set<t_tuple,t_comparator_0>;
t_ind_0 ind_0;
using iterator = t_ind_0::iterator;
struct context {
t_ind_0::operation_hints hints_0_lower;
t_ind_0::operation_hints hints_0_upper;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
if (ind_0.insert(t, h.hints_0_lower)) {
return true;
} else return false;
}
bool insert(const RamDomain* ramDomain) {
RamDomain data[2];
std::copy(ramDomain, ramDomain + 2, data);
const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
context h;
return insert(tuple, h);
}
bool insert(RamDomain a0,RamDomain a1) {
RamDomain data[2] = {a0,a1};
return insert(data);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(t, h.hints_0_lower);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(t, h.hints_0_lower);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> lowerUpperRange_00(const t_tuple& /* lower */, const t_tuple& /* upper */, context& /* h */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> lowerUpperRange_00(const t_tuple& /* lower */, const t_tuple& /* upper */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<t_ind_0::iterator> lowerUpperRange_11(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp == 0) {
    auto pos = ind_0.find(lower, h.hints_0_lower);
    auto fin = ind_0.end();
    if (pos != fin) {fin = pos; ++fin;}
    return make_range(pos, fin);
}
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_11(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_11(lower,upper,h);
}
bool empty() const {
return ind_0.empty();
}
std::vector<range<iterator>> partition() const {
return ind_0.getChunks(400);
}
void purge() {
ind_0.clear();
}
iterator begin() const {
return ind_0.begin();
}
iterator end() const {
return ind_0.end();
}
void printStatistics(std::ostream& o) const {
o << " arity 2 direct b-tree index 0 lex-order [0,1]\n";
ind_0.printStats(o);
}
};

class Sf_foo : public SouffleProgram {
private:
static inline bool regex_wrapper(const std::string& pattern, const std::string& text) {
   bool result = false; 
   try { result = std::regex_match(text, std::regex(pattern)); } catch(...) { 
     std::cerr << "warning: wrong pattern provided for match(\"" << pattern << "\",\"" << text << "\").\n";
}
   return result;
}
private:
static inline std::string substr_wrapper(const std::string& str, size_t idx, size_t len) {
   std::string result; 
   try { result = str.substr(idx,len); } catch(...) { 
     std::cerr << "warning: wrong index position provided by substr(\"";
     std::cerr << str << "\"," << (int32_t)idx << "," << (int32_t)len << ") functor.\n";
   } return result;
}
public:
// -- initialize symbol table --
SymbolTable symTable{
	R"_(inputSize)_",
	R"_(outputSize)_",
	R"_(communicationTime)_",
	R"_(many-kafka)_",
	R"_(none)_",
	R"_(runTime)_",
	R"_(stringBytesProduced)_",
	R"_(relationTermsProduced)_",
	R"_(NR)_",
	R"_(half-complete-graph-)_",
};// -- initialize record table --
RecordTable recordTable;
// -- Table: CommunicationTime
Own<t_btree_iii__0_1_2__100__111> rel_1_CommunicationTime = mk<t_btree_iii__0_1_2__100__111>();
souffle::RelationWrapper<0,t_btree_iii__0_1_2__100__111,Tuple<RamDomain,3>,3,0> wrapper_rel_1_CommunicationTime;
// -- Table: InputSize
Own<t_btree_ii__0_1__11__10> rel_2_InputSize = mk<t_btree_ii__0_1__11__10>();
souffle::RelationWrapper<1,t_btree_ii__0_1__11__10,Tuple<RamDomain,2>,2,0> wrapper_rel_2_InputSize;
// -- Table: KafkaMode
Own<t_btree_ii__1_0__11__01> rel_3_KafkaMode = mk<t_btree_ii__1_0__11__01>();
souffle::RelationWrapper<2,t_btree_ii__1_0__11__01,Tuple<RamDomain,2>,2,0> wrapper_rel_3_KafkaMode;
// -- Table: MetricTypeA
Own<t_btree_iiiiiiiiiiii__0_1_2_3_4_5_6_7_8_9_10_11__111111111111> rel_4_MetricTypeA = mk<t_btree_iiiiiiiiiiii__0_1_2_3_4_5_6_7_8_9_10_11__111111111111>();
souffle::RelationWrapper<3,t_btree_iiiiiiiiiiii__0_1_2_3_4_5_6_7_8_9_10_11__111111111111,Tuple<RamDomain,12>,12,0> wrapper_rel_4_MetricTypeA;
// -- Table: MetricTypeB
Own<t_btree_iiiiiiiiiiiii__0_1_2_3_4_5_6_7_8_9_10_11_12__1111111111111> rel_5_MetricTypeB = mk<t_btree_iiiiiiiiiiiii__0_1_2_3_4_5_6_7_8_9_10_11_12__1111111111111>();
souffle::RelationWrapper<4,t_btree_iiiiiiiiiiiii__0_1_2_3_4_5_6_7_8_9_10_11_12__1111111111111,Tuple<RamDomain,13>,13,0> wrapper_rel_5_MetricTypeB;
// -- Table: OutputSize
Own<t_btree_ii__0_1__11__10> rel_6_OutputSize = mk<t_btree_ii__0_1__11__10>();
souffle::RelationWrapper<5,t_btree_ii__0_1__11__10,Tuple<RamDomain,2>,2,0> wrapper_rel_6_OutputSize;
// -- Table: Prefix
Own<t_btree_iiiiiiiii__1_0_2_3_4_5_6_7_8__010000000__111111111> rel_7_Prefix = mk<t_btree_iiiiiiiii__1_0_2_3_4_5_6_7_8__010000000__111111111>();
souffle::RelationWrapper<6,t_btree_iiiiiiiii__1_0_2_3_4_5_6_7_8__010000000__111111111,Tuple<RamDomain,9>,9,0> wrapper_rel_7_Prefix;
// -- Table: RelationTuplesProduced
Own<t_btree_iii__0_1_2__100__111> rel_8_RelationTuplesProduced = mk<t_btree_iii__0_1_2__100__111>();
souffle::RelationWrapper<7,t_btree_iii__0_1_2__100__111,Tuple<RamDomain,3>,3,0> wrapper_rel_8_RelationTuplesProduced;
// -- Table: Results1
Own<t_btree_iiiiiiiiiiiiiiii__0_1_2_3_4_5_6_7_8_9_10_11_12_13_14_15__1111111111111111> rel_9_Results1 = mk<t_btree_iiiiiiiiiiiiiiii__0_1_2_3_4_5_6_7_8_9_10_11_12_13_14_15__1111111111111111>();
souffle::RelationWrapper<8,t_btree_iiiiiiiiiiiiiiii__0_1_2_3_4_5_6_7_8_9_10_11_12_13_14_15__1111111111111111,Tuple<RamDomain,16>,16,0> wrapper_rel_9_Results1;
// -- Table: RunTime
Own<t_btree_iii__0_1_2__100__110__111> rel_10_RunTime = mk<t_btree_iii__0_1_2__100__110__111>();
souffle::RelationWrapper<9,t_btree_iii__0_1_2__100__110__111,Tuple<RamDomain,3>,3,0> wrapper_rel_10_RunTime;
// -- Table: StringBytesProduced
Own<t_btree_iii__0_1_2__100__111> rel_11_StringBytesProduced = mk<t_btree_iii__0_1_2__100__111>();
souffle::RelationWrapper<10,t_btree_iii__0_1_2__100__111,Tuple<RamDomain,3>,3,0> wrapper_rel_11_StringBytesProduced;
// -- Table: SuffixTypeA
Own<t_btree_iii__1_0_2__010__111> rel_12_SuffixTypeA = mk<t_btree_iii__1_0_2__010__111>();
souffle::RelationWrapper<11,t_btree_iii__1_0_2__010__111,Tuple<RamDomain,3>,3,0> wrapper_rel_12_SuffixTypeA;
// -- Table: SuffixTypeB
Own<t_btree_iiii__1_0_2_3__1111__0100> rel_13_SuffixTypeB = mk<t_btree_iiii__1_0_2_3__1111__0100>();
souffle::RelationWrapper<12,t_btree_iiii__1_0_2_3__1111__0100,Tuple<RamDomain,4>,4,0> wrapper_rel_13_SuffixTypeB;
// -- Table: TotalCommunicationTime
Own<t_btree_ii__0_1__11__10> rel_14_TotalCommunicationTime = mk<t_btree_ii__0_1__11__10>();
souffle::RelationWrapper<13,t_btree_ii__0_1__11__10,Tuple<RamDomain,2>,2,0> wrapper_rel_14_TotalCommunicationTime;
// -- Table: TotalComputationTime
Own<t_btree_ii__0_1__11__10> rel_15_TotalComputationTime = mk<t_btree_ii__0_1__11__10>();
souffle::RelationWrapper<14,t_btree_ii__0_1__11__10,Tuple<RamDomain,2>,2,0> wrapper_rel_15_TotalComputationTime;
// -- Table: TotalRelationTuplesProduced
Own<t_btree_ii__0_1__11__10> rel_16_TotalRelationTuplesProduced = mk<t_btree_ii__0_1__11__10>();
souffle::RelationWrapper<15,t_btree_ii__0_1__11__10,Tuple<RamDomain,2>,2,0> wrapper_rel_16_TotalRelationTuplesProduced;
// -- Table: TotalRunTime
Own<t_btree_ii__0_1__11> rel_17_TotalRunTime = mk<t_btree_ii__0_1__11>();
souffle::RelationWrapper<16,t_btree_ii__0_1__11,Tuple<RamDomain,2>,2,0> wrapper_rel_17_TotalRunTime;
// -- Table: TotalStringBytesProduced
Own<t_btree_ii__0_1__11__10> rel_18_TotalStringBytesProduced = mk<t_btree_ii__0_1__11__10>();
souffle::RelationWrapper<17,t_btree_ii__0_1__11__10,Tuple<RamDomain,2>,2,0> wrapper_rel_18_TotalStringBytesProduced;
public:
Sf_foo() : 
wrapper_rel_1_CommunicationTime(*rel_1_CommunicationTime,symTable,"CommunicationTime",std::array<const char *,3>{{"i:number","s:symbol","i:number"}},std::array<const char *,3>{{"unique_id","stratum_index","runtime_value"}}),

wrapper_rel_2_InputSize(*rel_2_InputSize,symTable,"InputSize",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"unique_id","tuple_count"}}),

wrapper_rel_3_KafkaMode(*rel_3_KafkaMode,symTable,"KafkaMode",std::array<const char *,2>{{"i:number","s:symbol"}},std::array<const char *,2>{{"unique_id","kafka_mode"}}),

wrapper_rel_4_MetricTypeA(*rel_4_MetricTypeA,symTable,"MetricTypeA",std::array<const char *,12>{{"s:symbol","s:symbol","i:number","s:symbol","s:symbol","s:symbol","s:symbol","s:symbol","i:number","i:number","s:symbol","i:number"}},std::array<const char *,12>{{"benchmark_program","data_type","split_size","join_type","kafka_mode","evaluation_algorithm","undefined_todo","dataset_name","thread_size","unique_id","metric_name","metric_value"}}),

wrapper_rel_5_MetricTypeB(*rel_5_MetricTypeB,symTable,"MetricTypeB",std::array<const char *,13>{{"s:symbol","s:symbol","i:number","s:symbol","s:symbol","s:symbol","s:symbol","s:symbol","i:number","i:number","s:symbol","s:symbol","i:number"}},std::array<const char *,13>{{"benchmark_program","data_type","split_size","join_type","kafka_mode","evaluation_algorithm","undefined_todo","dataset_name","thread_size","unique_id","metric_name","metric_key","metric_value"}}),

wrapper_rel_6_OutputSize(*rel_6_OutputSize,symTable,"OutputSize",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"unique_id","tuple_count"}}),

wrapper_rel_7_Prefix(*rel_7_Prefix,symTable,"Prefix",std::array<const char *,9>{{"i:number","s:symbol","s:symbol","i:number","s:symbol","s:symbol","s:symbol","s:symbol","i:number"}},std::array<const char *,9>{{"unique_id","benchmark_program","data_type","split_size","join_type","kafka_mode","evaluation_algorithm","dataset_name","thread_size"}}),

wrapper_rel_8_RelationTuplesProduced(*rel_8_RelationTuplesProduced,symTable,"RelationTuplesProduced",std::array<const char *,3>{{"i:number","s:symbol","i:number"}},std::array<const char *,3>{{"unique_id","relation_name","tuple_count"}}),

wrapper_rel_9_Results1(*rel_9_Results1,symTable,"Results1",std::array<const char *,16>{{"i:number","s:symbol","s:symbol","i:number","s:symbol","s:symbol","s:symbol","s:symbol","i:number","i:number","i:number","i:number","i:number","i:number","i:number","i:number"}},std::array<const char *,16>{{"unique_id","benchmark_program","data_type","split_size","join_type","kafka_mode","evaluation_algorithm","dataset_name","thread_size","input_size","output_size","run_time","communication_time","computation_time","tuple_count","string_bytes"}}),

wrapper_rel_10_RunTime(*rel_10_RunTime,symTable,"RunTime",std::array<const char *,3>{{"i:number","s:symbol","i:number"}},std::array<const char *,3>{{"unique_id","stratum_index","runtime_value"}}),

wrapper_rel_11_StringBytesProduced(*rel_11_StringBytesProduced,symTable,"StringBytesProduced",std::array<const char *,3>{{"i:number","s:symbol","i:number"}},std::array<const char *,3>{{"unique_id","stratum_index","byte_count"}}),

wrapper_rel_12_SuffixTypeA(*rel_12_SuffixTypeA,symTable,"SuffixTypeA",std::array<const char *,3>{{"i:number","s:symbol","i:number"}},std::array<const char *,3>{{"unique_id","metric_name","metric_value"}}),

wrapper_rel_13_SuffixTypeB(*rel_13_SuffixTypeB,symTable,"SuffixTypeB",std::array<const char *,4>{{"i:number","s:symbol","s:symbol","i:number"}},std::array<const char *,4>{{"unique_id","metric_name","metric_key","metric_value"}}),

wrapper_rel_14_TotalCommunicationTime(*rel_14_TotalCommunicationTime,symTable,"TotalCommunicationTime",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"unique_id","communication_time"}}),

wrapper_rel_15_TotalComputationTime(*rel_15_TotalComputationTime,symTable,"TotalComputationTime",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"unique_id","computation_time"}}),

wrapper_rel_16_TotalRelationTuplesProduced(*rel_16_TotalRelationTuplesProduced,symTable,"TotalRelationTuplesProduced",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"unique_id","tuple_count"}}),

wrapper_rel_17_TotalRunTime(*rel_17_TotalRunTime,symTable,"TotalRunTime",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"unique_id","run_time"}}),

wrapper_rel_18_TotalStringBytesProduced(*rel_18_TotalStringBytesProduced,symTable,"TotalStringBytesProduced",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"unique_id","string_bytes"}}){
addRelation("CommunicationTime",&wrapper_rel_1_CommunicationTime,false,false);
addRelation("InputSize",&wrapper_rel_2_InputSize,false,false);
addRelation("KafkaMode",&wrapper_rel_3_KafkaMode,false,false);
addRelation("MetricTypeA",&wrapper_rel_4_MetricTypeA,true,false);
addRelation("MetricTypeB",&wrapper_rel_5_MetricTypeB,true,false);
addRelation("OutputSize",&wrapper_rel_6_OutputSize,false,false);
addRelation("Prefix",&wrapper_rel_7_Prefix,false,false);
addRelation("RelationTuplesProduced",&wrapper_rel_8_RelationTuplesProduced,false,false);
addRelation("Results1",&wrapper_rel_9_Results1,false,true);
addRelation("RunTime",&wrapper_rel_10_RunTime,false,false);
addRelation("StringBytesProduced",&wrapper_rel_11_StringBytesProduced,false,false);
addRelation("SuffixTypeA",&wrapper_rel_12_SuffixTypeA,false,false);
addRelation("SuffixTypeB",&wrapper_rel_13_SuffixTypeB,false,false);
addRelation("TotalCommunicationTime",&wrapper_rel_14_TotalCommunicationTime,false,false);
addRelation("TotalComputationTime",&wrapper_rel_15_TotalComputationTime,false,false);
addRelation("TotalRelationTuplesProduced",&wrapper_rel_16_TotalRelationTuplesProduced,false,false);
addRelation("TotalRunTime",&wrapper_rel_17_TotalRunTime,false,false);
addRelation("TotalStringBytesProduced",&wrapper_rel_18_TotalStringBytesProduced,false,false);
}
~Sf_foo() {
}
private:
std::string inputDirectory;
std::string outputDirectory;
bool performIO;
std::atomic<RamDomain> ctr{};

std::atomic<size_t> iter{};
void runFunction(std::string inputDirectoryArg = "", std::string outputDirectoryArg = "", bool performIOArg = false) {
this->inputDirectory = inputDirectoryArg;
this->outputDirectory = outputDirectoryArg;
this->performIO = performIOArg;
SignalHandler::instance()->set();
#if defined(_OPENMP)
if (getNumThreads() > 0) {omp_set_num_threads(getNumThreads());}
#endif

// -- query evaluation --
{
 std::vector<RamDomain> args, ret;
subroutine_0(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_1(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_10(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_11(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_12(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_13(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_14(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_15(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_16(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_17(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_2(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_3(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_4(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_5(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_6(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_7(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_8(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_9(args, ret);
}

// -- relation hint statistics --
SignalHandler::instance()->reset();
}
public:
void run() override { runFunction("", "", false); }
public:
void runAll(std::string inputDirectoryArg = "", std::string outputDirectoryArg = "") override { runFunction(inputDirectoryArg, outputDirectoryArg, true);
}
public:
void printAll(std::string outputDirectoryArg = "") override {
try {std::map<std::string, std::string> directiveMap({{"IO","file"},{"attributeNames","unique_id,benchmark_program,data_type,split_size,join_type,kafka_mode,evaluation_algorithm,dataset_name,thread_size,input_size,output_size,run_time,communication_time,computation_time,tuple_count,string_bytes"},{"delimiter",","},{"headers","true"},{"name","Results1"},{"operation","output"},{"output-dir","/Users/lyndonhenry/Downloads"},{"params","{\"records\": {}, \"relation\": {\"arity\": 16, \"auxArity\": 0, \"params\": [\"unique_id\", \"benchmark_program\", \"data_type\", \"split_size\", \"join_type\", \"kafka_mode\", \"evaluation_algorithm\", \"dataset_name\", \"thread_size\", \"input_size\", \"output_size\", \"run_time\", \"communication_time\", \"computation_time\", \"tuple_count\", \"string_bytes\"]}}"},{"types","{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": 16, \"auxArity\": 0, \"types\": [\"i:number\", \"s:symbol\", \"s:symbol\", \"i:number\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"i:number\", \"i:number\", \"i:number\", \"i:number\", \"i:number\", \"i:number\", \"i:number\", \"i:number\"]}}"}});
if (!outputDirectoryArg.empty()) {directiveMap["output-dir"] = outputDirectoryArg;}
IOSystem::getInstance().getWriter(directiveMap, symTable, recordTable)->writeAll(*rel_9_Results1);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
public:
void loadAll(std::string inputDirectoryArg = "") override {
try {std::map<std::string, std::string> directiveMap({{"IO","file"},{"attributeNames","benchmark_program,data_type,split_size,join_type,kafka_mode,evaluation_algorithm,undefined_todo,dataset_name,thread_size,unique_id,metric_name,metric_key,metric_value"},{"delimiter",","},{"fact-dir","/Users/lyndonhenry/Downloads"},{"name","MetricTypeB"},{"operation","input"},{"params","{\"records\": {}, \"relation\": {\"arity\": 13, \"auxArity\": 0, \"params\": [\"benchmark_program\", \"data_type\", \"split_size\", \"join_type\", \"kafka_mode\", \"evaluation_algorithm\", \"undefined_todo\", \"dataset_name\", \"thread_size\", \"unique_id\", \"metric_name\", \"metric_key\", \"metric_value\"]}}"},{"types","{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": 13, \"auxArity\": 0, \"types\": [\"s:symbol\", \"s:symbol\", \"i:number\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"i:number\", \"i:number\", \"s:symbol\", \"s:symbol\", \"i:number\"]}}"}});
if (!inputDirectoryArg.empty()) {directiveMap["fact-dir"] = inputDirectoryArg;}
IOSystem::getInstance().getReader(directiveMap, symTable, recordTable)->readAll(*rel_5_MetricTypeB);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"IO","file"},{"attributeNames","benchmark_program,data_type,split_size,join_type,kafka_mode,evaluation_algorithm,undefined_todo,dataset_name,thread_size,unique_id,metric_name,metric_value"},{"delimiter",","},{"fact-dir","/Users/lyndonhenry/Downloads"},{"name","MetricTypeA"},{"operation","input"},{"params","{\"records\": {}, \"relation\": {\"arity\": 12, \"auxArity\": 0, \"params\": [\"benchmark_program\", \"data_type\", \"split_size\", \"join_type\", \"kafka_mode\", \"evaluation_algorithm\", \"undefined_todo\", \"dataset_name\", \"thread_size\", \"unique_id\", \"metric_name\", \"metric_value\"]}}"},{"types","{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": 12, \"auxArity\": 0, \"types\": [\"s:symbol\", \"s:symbol\", \"i:number\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"i:number\", \"i:number\", \"s:symbol\", \"i:number\"]}}"}});
if (!inputDirectoryArg.empty()) {directiveMap["fact-dir"] = inputDirectoryArg;}
IOSystem::getInstance().getReader(directiveMap, symTable, recordTable)->readAll(*rel_4_MetricTypeA);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
public:
void dumpInputs() override {
try {std::map<std::string, std::string> rwOperation;
rwOperation["IO"] = "stdout";
rwOperation["name"] = "MetricTypeB";
rwOperation["types"] = "{\"relation\": {\"arity\": 13, \"auxArity\": 0, \"types\": [\"s:symbol\", \"s:symbol\", \"i:number\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"i:number\", \"i:number\", \"s:symbol\", \"s:symbol\", \"i:number\"]}}";
IOSystem::getInstance().getWriter(rwOperation, symTable, recordTable)->writeAll(*rel_5_MetricTypeB);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> rwOperation;
rwOperation["IO"] = "stdout";
rwOperation["name"] = "MetricTypeA";
rwOperation["types"] = "{\"relation\": {\"arity\": 12, \"auxArity\": 0, \"types\": [\"s:symbol\", \"s:symbol\", \"i:number\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"i:number\", \"i:number\", \"s:symbol\", \"i:number\"]}}";
IOSystem::getInstance().getWriter(rwOperation, symTable, recordTable)->writeAll(*rel_4_MetricTypeA);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
public:
void dumpOutputs() override {
try {std::map<std::string, std::string> rwOperation;
rwOperation["IO"] = "stdout";
rwOperation["name"] = "Results1";
rwOperation["types"] = "{\"relation\": {\"arity\": 16, \"auxArity\": 0, \"types\": [\"i:number\", \"s:symbol\", \"s:symbol\", \"i:number\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"i:number\", \"i:number\", \"i:number\", \"i:number\", \"i:number\", \"i:number\", \"i:number\", \"i:number\"]}}";
IOSystem::getInstance().getWriter(rwOperation, symTable, recordTable)->writeAll(*rel_9_Results1);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
public:
SymbolTable& getSymbolTable() override {
return symTable;
}
void executeSubroutine(std::string name, const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) override {
if (name == "stratum_0") {
subroutine_0(args, ret);
return;}
if (name == "stratum_1") {
subroutine_1(args, ret);
return;}
if (name == "stratum_10") {
subroutine_2(args, ret);
return;}
if (name == "stratum_11") {
subroutine_3(args, ret);
return;}
if (name == "stratum_12") {
subroutine_4(args, ret);
return;}
if (name == "stratum_13") {
subroutine_5(args, ret);
return;}
if (name == "stratum_14") {
subroutine_6(args, ret);
return;}
if (name == "stratum_15") {
subroutine_7(args, ret);
return;}
if (name == "stratum_16") {
subroutine_8(args, ret);
return;}
if (name == "stratum_17") {
subroutine_9(args, ret);
return;}
if (name == "stratum_2") {
subroutine_10(args, ret);
return;}
if (name == "stratum_3") {
subroutine_11(args, ret);
return;}
if (name == "stratum_4") {
subroutine_12(args, ret);
return;}
if (name == "stratum_5") {
subroutine_13(args, ret);
return;}
if (name == "stratum_6") {
subroutine_14(args, ret);
return;}
if (name == "stratum_7") {
subroutine_15(args, ret);
return;}
if (name == "stratum_8") {
subroutine_16(args, ret);
return;}
if (name == "stratum_9") {
subroutine_17(args, ret);
return;}
fatal("unknown subroutine");
}
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_0(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"IO","file"},{"attributeNames","benchmark_program,data_type,split_size,join_type,kafka_mode,evaluation_algorithm,undefined_todo,dataset_name,thread_size,unique_id,metric_name,metric_value"},{"delimiter",","},{"fact-dir","/Users/lyndonhenry/Downloads"},{"name","MetricTypeA"},{"operation","input"},{"params","{\"records\": {}, \"relation\": {\"arity\": 12, \"auxArity\": 0, \"params\": [\"benchmark_program\", \"data_type\", \"split_size\", \"join_type\", \"kafka_mode\", \"evaluation_algorithm\", \"undefined_todo\", \"dataset_name\", \"thread_size\", \"unique_id\", \"metric_name\", \"metric_value\"]}}"},{"types","{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": 12, \"auxArity\": 0, \"types\": [\"s:symbol\", \"s:symbol\", \"i:number\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"i:number\", \"i:number\", \"s:symbol\", \"i:number\"]}}"}});
if (!inputDirectory.empty()) {directiveMap["fact-dir"] = inputDirectory;}
IOSystem::getInstance().getReader(directiveMap, symTable, recordTable)->readAll(*rel_4_MetricTypeA);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_1(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(SuffixTypeA(ui,mn,mv) :- 
   MetricTypeA(_,_,_,_,_,_,_,_,_,ui,mn,mv).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [123:1-124:41])_");
if(!(rel_4_MetricTypeA->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_12_SuffixTypeA_op_ctxt,rel_12_SuffixTypeA->createContext());
CREATE_OP_CONTEXT(rel_4_MetricTypeA_op_ctxt,rel_4_MetricTypeA->createContext());
for(const auto& env0 : *rel_4_MetricTypeA) {
Tuple<RamDomain,3> tuple{{ramBitCast(env0[9]),ramBitCast(env0[10]),ramBitCast(env0[11])}};
rel_12_SuffixTypeA->insert(tuple,READ_OP_CONTEXT(rel_12_SuffixTypeA_op_ctxt));
}
}
();}
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_2(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(RunTime(ui,si,rv) :- 
   SuffixTypeB(ui,"runTime",si,rv).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [160:1-161:33])_");
if(!(rel_13_SuffixTypeB->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_13_SuffixTypeB_op_ctxt,rel_13_SuffixTypeB->createContext());
CREATE_OP_CONTEXT(rel_10_RunTime_op_ctxt,rel_10_RunTime->createContext());
auto range = rel_13_SuffixTypeB->lowerUpperRange_0100(Tuple<RamDomain,4>{{ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast(RamSigned(5)), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,4>{{ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast(RamSigned(5)), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_13_SuffixTypeB_op_ctxt));
for(const auto& env0 : range) {
Tuple<RamDomain,3> tuple{{ramBitCast(env0[0]),ramBitCast(env0[2]),ramBitCast(env0[3])}};
rel_10_RunTime->insert(tuple,READ_OP_CONTEXT(rel_10_RunTime_op_ctxt));
}
}
();}
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_3(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(TotalRunTime(ui,0) :- 
   !KafkaMode(ui,"many-kafka"),
   KafkaMode(ui,_).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [213:1-215:17])_");
if(!(rel_3_KafkaMode->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_3_KafkaMode_op_ctxt,rel_3_KafkaMode->createContext());
CREATE_OP_CONTEXT(rel_17_TotalRunTime_op_ctxt,rel_17_TotalRunTime->createContext());
for(const auto& env0 : *rel_3_KafkaMode) {
if( !(rel_3_KafkaMode->contains(Tuple<RamDomain,2>{{ramBitCast(env0[0]),ramBitCast(RamSigned(3))}},READ_OP_CONTEXT(rel_3_KafkaMode_op_ctxt)))) {
Tuple<RamDomain,2> tuple{{ramBitCast(env0[0]),ramBitCast(RamSigned(0))}};
rel_17_TotalRunTime->insert(tuple,READ_OP_CONTEXT(rel_17_TotalRunTime_op_ctxt));
}
}
}
();}
SignalHandler::instance()->setMsg(R"_(TotalRunTime(ui,sum  x1 : { RunTime(ui,si, x1),si != "none" }) :- 
   KafkaMode(ui,"many-kafka").
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [209:1-211:49])_");
if(!(rel_3_KafkaMode->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_3_KafkaMode_op_ctxt,rel_3_KafkaMode->createContext());
CREATE_OP_CONTEXT(rel_17_TotalRunTime_op_ctxt,rel_17_TotalRunTime->createContext());
CREATE_OP_CONTEXT(rel_10_RunTime_op_ctxt,rel_10_RunTime->createContext());
auto range = rel_3_KafkaMode->lowerUpperRange_01(Tuple<RamDomain,2>{{ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast(RamSigned(3))}},Tuple<RamDomain,2>{{ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast(RamSigned(3))}},READ_OP_CONTEXT(rel_3_KafkaMode_op_ctxt));
for(const auto& env0 : range) {
Tuple<RamDomain,1> env1;
bool shouldRunNested = false;
shouldRunNested = true;
RamSigned res0 = 0;
auto range = rel_10_RunTime->lowerUpperRange_100(Tuple<RamDomain,3>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,3>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_10_RunTime_op_ctxt));
for(const auto& env1 : range) {
if( (ramBitCast<RamDomain>(env1[1]) != ramBitCast<RamDomain>(RamSigned(4)))) {
shouldRunNested = true;
res0 += ramBitCast<RamSigned>(env1[2]);
}
}
env1[0] = ramBitCast(res0);
if (shouldRunNested) {
Tuple<RamDomain,2> tuple{{ramBitCast(env0[0]),ramBitCast(env1[0])}};
rel_17_TotalRunTime->insert(tuple,READ_OP_CONTEXT(rel_17_TotalRunTime_op_ctxt));
}
}
}
();}
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_4(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(TotalComputationTime(ui,(rt-ct1)) :- 
   TotalRunTime(ui,rt),
   TotalCommunicationTime(ui,ct1).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [219:1-222:15])_");
if(!(rel_17_TotalRunTime->empty()) && !(rel_14_TotalCommunicationTime->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_17_TotalRunTime_op_ctxt,rel_17_TotalRunTime->createContext());
CREATE_OP_CONTEXT(rel_14_TotalCommunicationTime_op_ctxt,rel_14_TotalCommunicationTime->createContext());
CREATE_OP_CONTEXT(rel_15_TotalComputationTime_op_ctxt,rel_15_TotalComputationTime->createContext());
for(const auto& env0 : *rel_17_TotalRunTime) {
auto range = rel_14_TotalCommunicationTime->lowerUpperRange_10(Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_14_TotalCommunicationTime_op_ctxt));
for(const auto& env1 : range) {
Tuple<RamDomain,2> tuple{{ramBitCast(env0[0]),ramBitCast((ramBitCast<RamSigned>(env0[1]) - ramBitCast<RamSigned>(env1[1])))}};
rel_15_TotalComputationTime->insert(tuple,READ_OP_CONTEXT(rel_15_TotalComputationTime_op_ctxt));
}
}
}
();}
if (performIO) rel_17_TotalRunTime->purge();
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_5(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(StringBytesProduced(ui,si,bc) :- 
   SuffixTypeB(ui,"stringBytesProduced",si,bc).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [185:1-186:45])_");
if(!(rel_13_SuffixTypeB->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_13_SuffixTypeB_op_ctxt,rel_13_SuffixTypeB->createContext());
CREATE_OP_CONTEXT(rel_11_StringBytesProduced_op_ctxt,rel_11_StringBytesProduced->createContext());
auto range = rel_13_SuffixTypeB->lowerUpperRange_0100(Tuple<RamDomain,4>{{ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast(RamSigned(6)), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,4>{{ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast(RamSigned(6)), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_13_SuffixTypeB_op_ctxt));
for(const auto& env0 : range) {
Tuple<RamDomain,3> tuple{{ramBitCast(env0[0]),ramBitCast(env0[2]),ramBitCast(env0[3])}};
rel_11_StringBytesProduced->insert(tuple,READ_OP_CONTEXT(rel_11_StringBytesProduced_op_ctxt));
}
}
();}
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_6(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(TotalStringBytesProduced(ui,0) :- 
   !KafkaMode(ui,"many-kafka"),
   KafkaMode(ui,_).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [240:1-242:17])_");
if(!(rel_3_KafkaMode->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_3_KafkaMode_op_ctxt,rel_3_KafkaMode->createContext());
CREATE_OP_CONTEXT(rel_18_TotalStringBytesProduced_op_ctxt,rel_18_TotalStringBytesProduced->createContext());
for(const auto& env0 : *rel_3_KafkaMode) {
if( !(rel_3_KafkaMode->contains(Tuple<RamDomain,2>{{ramBitCast(env0[0]),ramBitCast(RamSigned(3))}},READ_OP_CONTEXT(rel_3_KafkaMode_op_ctxt)))) {
Tuple<RamDomain,2> tuple{{ramBitCast(env0[0]),ramBitCast(RamSigned(0))}};
rel_18_TotalStringBytesProduced->insert(tuple,READ_OP_CONTEXT(rel_18_TotalStringBytesProduced_op_ctxt));
}
}
}
();}
SignalHandler::instance()->setMsg(R"_(TotalStringBytesProduced(ui,sum  x3 : { StringBytesProduced(ui,_, x3) }) :- 
   KafkaMode(ui,"many-kafka").
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [236:1-238:46])_");
if(!(rel_3_KafkaMode->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_3_KafkaMode_op_ctxt,rel_3_KafkaMode->createContext());
CREATE_OP_CONTEXT(rel_11_StringBytesProduced_op_ctxt,rel_11_StringBytesProduced->createContext());
CREATE_OP_CONTEXT(rel_18_TotalStringBytesProduced_op_ctxt,rel_18_TotalStringBytesProduced->createContext());
auto range = rel_3_KafkaMode->lowerUpperRange_01(Tuple<RamDomain,2>{{ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast(RamSigned(3))}},Tuple<RamDomain,2>{{ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast(RamSigned(3))}},READ_OP_CONTEXT(rel_3_KafkaMode_op_ctxt));
for(const auto& env0 : range) {
Tuple<RamDomain,1> env1;
bool shouldRunNested = false;
shouldRunNested = true;
RamSigned res0 = 0;
auto range = rel_11_StringBytesProduced->lowerUpperRange_100(Tuple<RamDomain,3>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,3>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_11_StringBytesProduced_op_ctxt));
for(const auto& env1 : range) {
if( true) {
shouldRunNested = true;
res0 += ramBitCast<RamSigned>(env1[2]);
}
}
env1[0] = ramBitCast(res0);
if (shouldRunNested) {
Tuple<RamDomain,2> tuple{{ramBitCast(env0[0]),ramBitCast(env1[0])}};
rel_18_TotalStringBytesProduced->insert(tuple,READ_OP_CONTEXT(rel_18_TotalStringBytesProduced_op_ctxt));
}
}
}
();}
if (performIO) rel_11_StringBytesProduced->purge();
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_7(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(RelationTuplesProduced(ui,rn,tc) :- 
   SuffixTypeB(ui,"relationTermsProduced",rn,tc).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [194:1-195:47])_");
if(!(rel_13_SuffixTypeB->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_13_SuffixTypeB_op_ctxt,rel_13_SuffixTypeB->createContext());
CREATE_OP_CONTEXT(rel_8_RelationTuplesProduced_op_ctxt,rel_8_RelationTuplesProduced->createContext());
auto range = rel_13_SuffixTypeB->lowerUpperRange_0100(Tuple<RamDomain,4>{{ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast(RamSigned(7)), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,4>{{ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast(RamSigned(7)), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_13_SuffixTypeB_op_ctxt));
for(const auto& env0 : range) {
Tuple<RamDomain,3> tuple{{ramBitCast(env0[0]),ramBitCast(env0[2]),ramBitCast(env0[3])}};
rel_8_RelationTuplesProduced->insert(tuple,READ_OP_CONTEXT(rel_8_RelationTuplesProduced_op_ctxt));
}
}
();}
if (performIO) rel_13_SuffixTypeB->purge();
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_8(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(TotalRelationTuplesProduced(ui,0) :- 
   !KafkaMode(ui,"many-kafka"),
   KafkaMode(ui,_).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [230:1-232:17])_");
if(!(rel_3_KafkaMode->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_3_KafkaMode_op_ctxt,rel_3_KafkaMode->createContext());
CREATE_OP_CONTEXT(rel_16_TotalRelationTuplesProduced_op_ctxt,rel_16_TotalRelationTuplesProduced->createContext());
for(const auto& env0 : *rel_3_KafkaMode) {
if( !(rel_3_KafkaMode->contains(Tuple<RamDomain,2>{{ramBitCast(env0[0]),ramBitCast(RamSigned(3))}},READ_OP_CONTEXT(rel_3_KafkaMode_op_ctxt)))) {
Tuple<RamDomain,2> tuple{{ramBitCast(env0[0]),ramBitCast(RamSigned(0))}};
rel_16_TotalRelationTuplesProduced->insert(tuple,READ_OP_CONTEXT(rel_16_TotalRelationTuplesProduced_op_ctxt));
}
}
}
();}
SignalHandler::instance()->setMsg(R"_(TotalRelationTuplesProduced(ui,sum  x2 : { RelationTuplesProduced(ui,_, x2) }) :- 
   KafkaMode(ui,"many-kafka").
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [226:1-228:49])_");
if(!(rel_3_KafkaMode->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_3_KafkaMode_op_ctxt,rel_3_KafkaMode->createContext());
CREATE_OP_CONTEXT(rel_8_RelationTuplesProduced_op_ctxt,rel_8_RelationTuplesProduced->createContext());
CREATE_OP_CONTEXT(rel_16_TotalRelationTuplesProduced_op_ctxt,rel_16_TotalRelationTuplesProduced->createContext());
auto range = rel_3_KafkaMode->lowerUpperRange_01(Tuple<RamDomain,2>{{ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast(RamSigned(3))}},Tuple<RamDomain,2>{{ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast(RamSigned(3))}},READ_OP_CONTEXT(rel_3_KafkaMode_op_ctxt));
for(const auto& env0 : range) {
Tuple<RamDomain,1> env1;
bool shouldRunNested = false;
shouldRunNested = true;
RamSigned res0 = 0;
auto range = rel_8_RelationTuplesProduced->lowerUpperRange_100(Tuple<RamDomain,3>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,3>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_8_RelationTuplesProduced_op_ctxt));
for(const auto& env1 : range) {
if( true) {
shouldRunNested = true;
res0 += ramBitCast<RamSigned>(env1[2]);
}
}
env1[0] = ramBitCast(res0);
if (shouldRunNested) {
Tuple<RamDomain,2> tuple{{ramBitCast(env0[0]),ramBitCast(env1[0])}};
rel_16_TotalRelationTuplesProduced->insert(tuple,READ_OP_CONTEXT(rel_16_TotalRelationTuplesProduced_op_ctxt));
}
}
}
();}
if (performIO) rel_3_KafkaMode->purge();
if (performIO) rel_8_RelationTuplesProduced->purge();
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_9(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(Results1(ui,bp,dt,ss,jt,km,ea,dn,ts,is,os,rt,ct1,ct2,tc,sb) :- 
   bp = "NR",
   substr(dn,0,strlen("half-complete-graph-")) = "half-complete-graph-",
   Prefix(ui,bp,dt,ss,jt,km,ea,dn,ts),
   InputSize(ui,is),
   OutputSize(ui,os),
   RunTime(ui,"none",rt),
   TotalCommunicationTime(ui,ct1),
   TotalComputationTime(ui,ct2),
   TotalRelationTuplesProduced(ui,tc),
   TotalStringBytesProduced(ui,sb).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [265:1-275:33])_");
if(!(rel_18_TotalStringBytesProduced->empty()) && !(rel_16_TotalRelationTuplesProduced->empty()) && !(rel_15_TotalComputationTime->empty()) && !(rel_14_TotalCommunicationTime->empty()) && !(rel_10_RunTime->empty()) && !(rel_6_OutputSize->empty()) && !(rel_7_Prefix->empty()) && !(rel_2_InputSize->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_2_InputSize_op_ctxt,rel_2_InputSize->createContext());
CREATE_OP_CONTEXT(rel_7_Prefix_op_ctxt,rel_7_Prefix->createContext());
CREATE_OP_CONTEXT(rel_6_OutputSize_op_ctxt,rel_6_OutputSize->createContext());
CREATE_OP_CONTEXT(rel_14_TotalCommunicationTime_op_ctxt,rel_14_TotalCommunicationTime->createContext());
CREATE_OP_CONTEXT(rel_10_RunTime_op_ctxt,rel_10_RunTime->createContext());
CREATE_OP_CONTEXT(rel_15_TotalComputationTime_op_ctxt,rel_15_TotalComputationTime->createContext());
CREATE_OP_CONTEXT(rel_18_TotalStringBytesProduced_op_ctxt,rel_18_TotalStringBytesProduced->createContext());
CREATE_OP_CONTEXT(rel_16_TotalRelationTuplesProduced_op_ctxt,rel_16_TotalRelationTuplesProduced->createContext());
CREATE_OP_CONTEXT(rel_9_Results1_op_ctxt,rel_9_Results1->createContext());
auto range = rel_7_Prefix->lowerUpperRange_010000000(Tuple<RamDomain,9>{{ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast(RamSigned(8)), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,9>{{ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast(RamSigned(8)), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_7_Prefix_op_ctxt));
for(const auto& env0 : range) {
if( (ramBitCast<RamDomain>(symTable.lookup(substr_wrapper(symTable.resolve(env0[7]),(RamSigned(0)),(static_cast<RamSigned>(symTable.resolve(RamSigned(9)).size()))))) == ramBitCast<RamDomain>(RamSigned(9)))) {
auto range = rel_2_InputSize->lowerUpperRange_10(Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_2_InputSize_op_ctxt));
for(const auto& env1 : range) {
auto range = rel_6_OutputSize->lowerUpperRange_10(Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_6_OutputSize_op_ctxt));
for(const auto& env2 : range) {
auto range = rel_10_RunTime->lowerUpperRange_110(Tuple<RamDomain,3>{{ramBitCast(env0[0]), ramBitCast(RamSigned(4)), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,3>{{ramBitCast(env0[0]), ramBitCast(RamSigned(4)), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_10_RunTime_op_ctxt));
for(const auto& env3 : range) {
auto range = rel_14_TotalCommunicationTime->lowerUpperRange_10(Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_14_TotalCommunicationTime_op_ctxt));
for(const auto& env4 : range) {
auto range = rel_15_TotalComputationTime->lowerUpperRange_10(Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_15_TotalComputationTime_op_ctxt));
for(const auto& env5 : range) {
auto range = rel_16_TotalRelationTuplesProduced->lowerUpperRange_10(Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_16_TotalRelationTuplesProduced_op_ctxt));
for(const auto& env6 : range) {
auto range = rel_18_TotalStringBytesProduced->lowerUpperRange_10(Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,2>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_18_TotalStringBytesProduced_op_ctxt));
for(const auto& env7 : range) {
Tuple<RamDomain,16> tuple{{ramBitCast(env0[0]),ramBitCast(env0[1]),ramBitCast(env0[2]),ramBitCast(env0[3]),ramBitCast(env0[4]),ramBitCast(env0[5]),ramBitCast(env0[6]),ramBitCast(env0[7]),ramBitCast(env0[8]),ramBitCast(env1[1]),ramBitCast(env2[1]),ramBitCast(env3[2]),ramBitCast(env4[1]),ramBitCast(env5[1]),ramBitCast(env6[1]),ramBitCast(env7[1])}};
rel_9_Results1->insert(tuple,READ_OP_CONTEXT(rel_9_Results1_op_ctxt));
}
}
}
}
}
}
}
}
}
}
();}
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"IO","file"},{"attributeNames","unique_id,benchmark_program,data_type,split_size,join_type,kafka_mode,evaluation_algorithm,dataset_name,thread_size,input_size,output_size,run_time,communication_time,computation_time,tuple_count,string_bytes"},{"delimiter",","},{"headers","true"},{"name","Results1"},{"operation","output"},{"output-dir","/Users/lyndonhenry/Downloads"},{"params","{\"records\": {}, \"relation\": {\"arity\": 16, \"auxArity\": 0, \"params\": [\"unique_id\", \"benchmark_program\", \"data_type\", \"split_size\", \"join_type\", \"kafka_mode\", \"evaluation_algorithm\", \"dataset_name\", \"thread_size\", \"input_size\", \"output_size\", \"run_time\", \"communication_time\", \"computation_time\", \"tuple_count\", \"string_bytes\"]}}"},{"types","{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": 16, \"auxArity\": 0, \"types\": [\"i:number\", \"s:symbol\", \"s:symbol\", \"i:number\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"i:number\", \"i:number\", \"i:number\", \"i:number\", \"i:number\", \"i:number\", \"i:number\", \"i:number\"]}}"}});
if (!outputDirectory.empty()) {directiveMap["output-dir"] = outputDirectory;}
IOSystem::getInstance().getWriter(directiveMap, symTable, recordTable)->writeAll(*rel_9_Results1);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
if (performIO) rel_7_Prefix->purge();
if (performIO) rel_15_TotalComputationTime->purge();
if (performIO) rel_16_TotalRelationTuplesProduced->purge();
if (performIO) rel_18_TotalStringBytesProduced->purge();
if (performIO) rel_10_RunTime->purge();
if (performIO) rel_2_InputSize->purge();
if (performIO) rel_6_OutputSize->purge();
if (performIO) rel_14_TotalCommunicationTime->purge();
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_10(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(InputSize(ui,tc) :- 
   SuffixTypeA(ui,"inputSize",tc).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [168:1-169:32])_");
if(!(rel_12_SuffixTypeA->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_12_SuffixTypeA_op_ctxt,rel_12_SuffixTypeA->createContext());
CREATE_OP_CONTEXT(rel_2_InputSize_op_ctxt,rel_2_InputSize->createContext());
auto range = rel_12_SuffixTypeA->lowerUpperRange_010(Tuple<RamDomain,3>{{ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast(RamSigned(0)), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,3>{{ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast(RamSigned(0)), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_12_SuffixTypeA_op_ctxt));
for(const auto& env0 : range) {
Tuple<RamDomain,2> tuple{{ramBitCast(env0[0]),ramBitCast(env0[2])}};
rel_2_InputSize->insert(tuple,READ_OP_CONTEXT(rel_2_InputSize_op_ctxt));
}
}
();}
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_11(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(OutputSize(ui,tc) :- 
   SuffixTypeA(ui,"outputSize",tc).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [176:1-177:33])_");
if(!(rel_12_SuffixTypeA->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_12_SuffixTypeA_op_ctxt,rel_12_SuffixTypeA->createContext());
CREATE_OP_CONTEXT(rel_6_OutputSize_op_ctxt,rel_6_OutputSize->createContext());
auto range = rel_12_SuffixTypeA->lowerUpperRange_010(Tuple<RamDomain,3>{{ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast(RamSigned(1)), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,3>{{ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast(RamSigned(1)), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_12_SuffixTypeA_op_ctxt));
for(const auto& env0 : range) {
Tuple<RamDomain,2> tuple{{ramBitCast(env0[0]),ramBitCast(env0[2])}};
rel_6_OutputSize->insert(tuple,READ_OP_CONTEXT(rel_6_OutputSize_op_ctxt));
}
}
();}
if (performIO) rel_12_SuffixTypeA->purge();
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_12(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"IO","file"},{"attributeNames","benchmark_program,data_type,split_size,join_type,kafka_mode,evaluation_algorithm,undefined_todo,dataset_name,thread_size,unique_id,metric_name,metric_key,metric_value"},{"delimiter",","},{"fact-dir","/Users/lyndonhenry/Downloads"},{"name","MetricTypeB"},{"operation","input"},{"params","{\"records\": {}, \"relation\": {\"arity\": 13, \"auxArity\": 0, \"params\": [\"benchmark_program\", \"data_type\", \"split_size\", \"join_type\", \"kafka_mode\", \"evaluation_algorithm\", \"undefined_todo\", \"dataset_name\", \"thread_size\", \"unique_id\", \"metric_name\", \"metric_key\", \"metric_value\"]}}"},{"types","{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": 13, \"auxArity\": 0, \"types\": [\"s:symbol\", \"s:symbol\", \"i:number\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"s:symbol\", \"i:number\", \"i:number\", \"s:symbol\", \"s:symbol\", \"i:number\"]}}"}});
if (!inputDirectory.empty()) {directiveMap["fact-dir"] = inputDirectory;}
IOSystem::getInstance().getReader(directiveMap, symTable, recordTable)->readAll(*rel_5_MetricTypeB);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_13(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(Prefix(ui,bp,dt,ss,jt,km,ea,dn,ts) :- 
   MetricTypeA(bp,dt,ss,jt,km,ea,_,dn,ts,ui,_,_).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [49:1-51:49])_");
if(!(rel_4_MetricTypeA->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_4_MetricTypeA_op_ctxt,rel_4_MetricTypeA->createContext());
CREATE_OP_CONTEXT(rel_7_Prefix_op_ctxt,rel_7_Prefix->createContext());
for(const auto& env0 : *rel_4_MetricTypeA) {
Tuple<RamDomain,9> tuple{{ramBitCast(env0[9]),ramBitCast(env0[0]),ramBitCast(env0[1]),ramBitCast(env0[2]),ramBitCast(env0[3]),ramBitCast(env0[4]),ramBitCast(env0[5]),ramBitCast(env0[7]),ramBitCast(env0[8])}};
rel_7_Prefix->insert(tuple,READ_OP_CONTEXT(rel_7_Prefix_op_ctxt));
}
}
();}
SignalHandler::instance()->setMsg(R"_(Prefix(ui,bp,dt,ss,jt,km,ea,dn,ts) :- 
   MetricTypeB(bp,dt,ss,jt,km,ea,_,dn,ts,ui,_,_,_).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [49:1-51:49])_");
if(!(rel_5_MetricTypeB->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_7_Prefix_op_ctxt,rel_7_Prefix->createContext());
CREATE_OP_CONTEXT(rel_5_MetricTypeB_op_ctxt,rel_5_MetricTypeB->createContext());
for(const auto& env0 : *rel_5_MetricTypeB) {
Tuple<RamDomain,9> tuple{{ramBitCast(env0[9]),ramBitCast(env0[0]),ramBitCast(env0[1]),ramBitCast(env0[2]),ramBitCast(env0[3]),ramBitCast(env0[4]),ramBitCast(env0[5]),ramBitCast(env0[7]),ramBitCast(env0[8])}};
rel_7_Prefix->insert(tuple,READ_OP_CONTEXT(rel_7_Prefix_op_ctxt));
}
}
();}
if (performIO) rel_4_MetricTypeA->purge();
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_14(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(KafkaMode(ui,km) :- 
   Prefix(ui,_,_,_,_,km,_,_,_).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [90:1-91:29])_");
if(!(rel_7_Prefix->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_3_KafkaMode_op_ctxt,rel_3_KafkaMode->createContext());
CREATE_OP_CONTEXT(rel_7_Prefix_op_ctxt,rel_7_Prefix->createContext());
for(const auto& env0 : *rel_7_Prefix) {
Tuple<RamDomain,2> tuple{{ramBitCast(env0[0]),ramBitCast(env0[5])}};
rel_3_KafkaMode->insert(tuple,READ_OP_CONTEXT(rel_3_KafkaMode_op_ctxt));
}
}
();}
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_15(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(SuffixTypeB(ui,mn,mk,mv) :- 
   MetricTypeB(_,_,_,_,_,_,_,_,_,ui,mn,mk,mv).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [133:1-134:44])_");
if(!(rel_5_MetricTypeB->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_5_MetricTypeB_op_ctxt,rel_5_MetricTypeB->createContext());
CREATE_OP_CONTEXT(rel_13_SuffixTypeB_op_ctxt,rel_13_SuffixTypeB->createContext());
for(const auto& env0 : *rel_5_MetricTypeB) {
Tuple<RamDomain,4> tuple{{ramBitCast(env0[9]),ramBitCast(env0[10]),ramBitCast(env0[11]),ramBitCast(env0[12])}};
rel_13_SuffixTypeB->insert(tuple,READ_OP_CONTEXT(rel_13_SuffixTypeB_op_ctxt));
}
}
();}
if (performIO) rel_5_MetricTypeB->purge();
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_16(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(CommunicationTime(ui,si,rv) :- 
   SuffixTypeB(ui,"communicationTime",si,rv).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [142:1-143:43])_");
if(!(rel_13_SuffixTypeB->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_1_CommunicationTime_op_ctxt,rel_1_CommunicationTime->createContext());
CREATE_OP_CONTEXT(rel_13_SuffixTypeB_op_ctxt,rel_13_SuffixTypeB->createContext());
auto range = rel_13_SuffixTypeB->lowerUpperRange_0100(Tuple<RamDomain,4>{{ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast(RamSigned(2)), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,4>{{ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast(RamSigned(2)), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_13_SuffixTypeB_op_ctxt));
for(const auto& env0 : range) {
Tuple<RamDomain,3> tuple{{ramBitCast(env0[0]),ramBitCast(env0[2]),ramBitCast(env0[3])}};
rel_1_CommunicationTime->insert(tuple,READ_OP_CONTEXT(rel_1_CommunicationTime_op_ctxt));
}
}
();}
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_17(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
SignalHandler::instance()->setMsg(R"_(TotalCommunicationTime(ui,0) :- 
   !KafkaMode(ui,"many-kafka"),
   KafkaMode(ui,_).
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [203:1-205:17])_");
if(!(rel_3_KafkaMode->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_3_KafkaMode_op_ctxt,rel_3_KafkaMode->createContext());
CREATE_OP_CONTEXT(rel_14_TotalCommunicationTime_op_ctxt,rel_14_TotalCommunicationTime->createContext());
for(const auto& env0 : *rel_3_KafkaMode) {
if( !(rel_3_KafkaMode->contains(Tuple<RamDomain,2>{{ramBitCast(env0[0]),ramBitCast(RamSigned(3))}},READ_OP_CONTEXT(rel_3_KafkaMode_op_ctxt)))) {
Tuple<RamDomain,2> tuple{{ramBitCast(env0[0]),ramBitCast(RamSigned(0))}};
rel_14_TotalCommunicationTime->insert(tuple,READ_OP_CONTEXT(rel_14_TotalCommunicationTime_op_ctxt));
}
}
}
();}
SignalHandler::instance()->setMsg(R"_(TotalCommunicationTime(ui,sum  x0 : { CommunicationTime(ui,si, x0),si != "none" }) :- 
   KafkaMode(ui,"many-kafka").
in file /Users/lyndonhenry/souffle/kafka/generate-results.dl [199:1-201:59])_");
if(!(rel_3_KafkaMode->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_3_KafkaMode_op_ctxt,rel_3_KafkaMode->createContext());
CREATE_OP_CONTEXT(rel_1_CommunicationTime_op_ctxt,rel_1_CommunicationTime->createContext());
CREATE_OP_CONTEXT(rel_14_TotalCommunicationTime_op_ctxt,rel_14_TotalCommunicationTime->createContext());
auto range = rel_3_KafkaMode->lowerUpperRange_01(Tuple<RamDomain,2>{{ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast(RamSigned(3))}},Tuple<RamDomain,2>{{ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast(RamSigned(3))}},READ_OP_CONTEXT(rel_3_KafkaMode_op_ctxt));
for(const auto& env0 : range) {
Tuple<RamDomain,1> env1;
bool shouldRunNested = false;
shouldRunNested = true;
RamSigned res0 = 0;
auto range = rel_1_CommunicationTime->lowerUpperRange_100(Tuple<RamDomain,3>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MIN_RAM_SIGNED), ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},Tuple<RamDomain,3>{{ramBitCast(env0[0]), ramBitCast<RamDomain>(MAX_RAM_SIGNED), ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},READ_OP_CONTEXT(rel_1_CommunicationTime_op_ctxt));
for(const auto& env1 : range) {
if( (ramBitCast<RamDomain>(env1[1]) != ramBitCast<RamDomain>(RamSigned(4)))) {
shouldRunNested = true;
res0 += ramBitCast<RamSigned>(env1[2]);
}
}
env1[0] = ramBitCast(res0);
if (shouldRunNested) {
Tuple<RamDomain,2> tuple{{ramBitCast(env0[0]),ramBitCast(env1[0])}};
rel_14_TotalCommunicationTime->insert(tuple,READ_OP_CONTEXT(rel_14_TotalCommunicationTime_op_ctxt));
}
}
}
();}
if (performIO) rel_1_CommunicationTime->purge();
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
};
SouffleProgram *newInstance_foo(){return new Sf_foo;}
SymbolTable *getST_foo(SouffleProgram *p){return &reinterpret_cast<Sf_foo*>(p)->symTable;}

#ifdef __EMBEDDED_SOUFFLE__
class factory_Sf_foo: public souffle::ProgramFactory {
SouffleProgram *newInstance() {
return new Sf_foo();
};
public:
factory_Sf_foo() : ProgramFactory("foo"){}
};
extern "C" {
factory_Sf_foo __factory_Sf_foo_instance;
}
}
#else
}
int main(int argc, char** argv)
{
try{
souffle::CmdOptions opt(R"(/Users/lyndonhenry/souffle/kafka/generate-results.dl)",
R"()",
R"()",
false,
R"()",
1);
if (!opt.parse(argc,argv)) return 1;
souffle::Sf_foo obj;
#if defined(_OPENMP) 
obj.setNumThreads(opt.getNumJobs());

#endif
obj.runAll(opt.getInputFileDir(), opt.getOutputFileDir());
return 0;
} catch(std::exception &e) { souffle::SignalHandler::instance()->error(e.what());}
}

#endif
