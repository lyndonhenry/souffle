
#include "souffle/CompiledSouffle.h"

extern "C" {
}

namespace souffle {
using namespace ram;
struct t_btree_2__0_1__3 {
using t_tuple = Tuple<RamDomain, 2>;
using t_ind_0 = btree_set<t_tuple, index_utils::comparator<0,1>>;
t_ind_0 ind_0;
using iterator = t_ind_0::iterator;
struct context {
t_ind_0::operation_hints hints_0;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
if (ind_0.insert(t, h.hints_0)) {
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
template <typename T>
void insertAll(T& other) {
for (auto const& cur : other) {
insert(cur);
}
}
void insertAll(t_btree_2__0_1__3& other) {
ind_0.insertAll(other.ind_0);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(t, h.hints_0);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(t, h.hints_0);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> equalRange_0(const t_tuple& t, context& h) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> equalRange_0(const t_tuple& t) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<t_ind_0::iterator> equalRange_3(const t_tuple& t, context& h) const {
auto pos = ind_0.find(t, h.hints_0);
auto fin = ind_0.end();
if (pos != fin) {fin = pos; ++fin;}
return make_range(pos, fin);
}
range<t_ind_0::iterator> equalRange_3(const t_tuple& t) const {
context h;
return equalRange_3(t, h);
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
void printHintStatistics(std::ostream& o, const std::string prefix) const {
const auto& stats_0 = ind_0.getHintStatistics();
o << prefix << "arity 2 direct b-tree index [0,1]: (hits/misses/total)\n";
o << prefix << "Insert: " << stats_0.inserts.getHits() << "/" << stats_0.inserts.getMisses() << "/" << stats_0.inserts.getAccesses() << "\n";
o << prefix << "Contains: " << stats_0.contains.getHits() << "/" << stats_0.contains.getMisses() << "/" << stats_0.contains.getAccesses() << "\n";
o << prefix << "Lower-bound: " << stats_0.lower_bound.getHits() << "/" << stats_0.lower_bound.getMisses() << "/" << stats_0.lower_bound.getAccesses() << "\n";
o << prefix << "Upper-bound: " << stats_0.upper_bound.getHits() << "/" << stats_0.upper_bound.getMisses() << "/" << stats_0.upper_bound.getAccesses() << "\n";
}
};
struct t_btree_2__0_1__1__3 {
using t_tuple = Tuple<RamDomain, 2>;
using t_ind_0 = btree_set<t_tuple, index_utils::comparator<0,1>>;
t_ind_0 ind_0;
using iterator = t_ind_0::iterator;
struct context {
t_ind_0::operation_hints hints_0;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
if (ind_0.insert(t, h.hints_0)) {
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
template <typename T>
void insertAll(T& other) {
for (auto const& cur : other) {
insert(cur);
}
}
void insertAll(t_btree_2__0_1__1__3& other) {
ind_0.insertAll(other.ind_0);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(t, h.hints_0);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(t, h.hints_0);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> equalRange_0(const t_tuple& t, context& h) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> equalRange_0(const t_tuple& t) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<t_ind_0::iterator> equalRange_1(const t_tuple& t, context& h) const {
t_tuple low(t); t_tuple high(t);
low[1] = MIN_RAM_DOMAIN;
high[1] = MAX_RAM_DOMAIN;
return make_range(ind_0.lower_bound(low, h.hints_0), ind_0.upper_bound(high, h.hints_0));
}
range<t_ind_0::iterator> equalRange_1(const t_tuple& t) const {
context h;
return equalRange_1(t, h);
}
range<t_ind_0::iterator> equalRange_3(const t_tuple& t, context& h) const {
auto pos = ind_0.find(t, h.hints_0);
auto fin = ind_0.end();
if (pos != fin) {fin = pos; ++fin;}
return make_range(pos, fin);
}
range<t_ind_0::iterator> equalRange_3(const t_tuple& t) const {
context h;
return equalRange_3(t, h);
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
void printHintStatistics(std::ostream& o, const std::string prefix) const {
const auto& stats_0 = ind_0.getHintStatistics();
o << prefix << "arity 2 direct b-tree index [0,1]: (hits/misses/total)\n";
o << prefix << "Insert: " << stats_0.inserts.getHits() << "/" << stats_0.inserts.getMisses() << "/" << stats_0.inserts.getAccesses() << "\n";
o << prefix << "Contains: " << stats_0.contains.getHits() << "/" << stats_0.contains.getMisses() << "/" << stats_0.contains.getAccesses() << "\n";
o << prefix << "Lower-bound: " << stats_0.lower_bound.getHits() << "/" << stats_0.lower_bound.getMisses() << "/" << stats_0.lower_bound.getAccesses() << "\n";
o << prefix << "Upper-bound: " << stats_0.upper_bound.getHits() << "/" << stats_0.upper_bound.getMisses() << "/" << stats_0.upper_bound.getAccesses() << "\n";
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
private:
static inline RamDomain wrapper_tonumber(const std::string& str) {
   RamDomain result=0; 
   try { result = stord(str); } catch(...) { 
     std::cerr << "error: wrong string provided by to_number(\"";
     std::cerr << str << "\") functor.\n";
     raise(SIGFPE);
   } return result;
}
public:
// -- initialize symbol table --
SymbolTable symTable
;// -- Table: @delta_AggregatedSender
std::unique_ptr<t_btree_2__0_1__3> rel_1_delta_AggregatedSender = std::make_unique<t_btree_2__0_1__3>();
// -- Table: @delta_InputOutput
std::unique_ptr<t_btree_2__0_1__1__3> rel_2_delta_InputOutput = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @delta_InputOutputReceive
std::unique_ptr<t_btree_2__0_1__1__3> rel_3_delta_InputOutputReceive = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @delta_InputOutputReceiveSend
std::unique_ptr<t_btree_2__0_1__1__3> rel_4_delta_InputOutputReceiveSend = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @delta_InputOutputSend
std::unique_ptr<t_btree_2__0_1__1__3> rel_5_delta_InputOutputSend = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @delta_InputReceiveSend
std::unique_ptr<t_btree_2__0_1__1__3> rel_6_delta_InputReceiveSend = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @delta_InputSend
std::unique_ptr<t_btree_2__0_1__1__3> rel_7_delta_InputSend = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @delta_NegatedSender
std::unique_ptr<t_btree_2__0_1__3> rel_8_delta_NegatedSender = std::make_unique<t_btree_2__0_1__3>();
// -- Table: @delta_Output
std::unique_ptr<t_btree_2__0_1__1__3> rel_9_delta_Output = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @delta_OutputReceive
std::unique_ptr<t_btree_2__0_1__1__3> rel_10_delta_OutputReceive = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @delta_OutputReceiveSend
std::unique_ptr<t_btree_2__0_1__1__3> rel_11_delta_OutputReceiveSend = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @delta_OutputSend
std::unique_ptr<t_btree_2__0_1__1__3> rel_12_delta_OutputSend = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @delta_Receiver
std::unique_ptr<t_btree_2__0_1__1__3> rel_13_delta_Receiver = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @delta_Send
std::unique_ptr<t_btree_2__0_1__1__3> rel_14_delta_Send = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @delta_Sender
std::unique_ptr<t_btree_2__0_1__3> rel_15_delta_Sender = std::make_unique<t_btree_2__0_1__3>();
// -- Table: @new_AggregatedSender
std::unique_ptr<t_btree_2__0_1__3> rel_16_new_AggregatedSender = std::make_unique<t_btree_2__0_1__3>();
// -- Table: @new_InputOutput
std::unique_ptr<t_btree_2__0_1__1__3> rel_17_new_InputOutput = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @new_InputOutputReceive
std::unique_ptr<t_btree_2__0_1__1__3> rel_18_new_InputOutputReceive = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @new_InputOutputReceiveSend
std::unique_ptr<t_btree_2__0_1__1__3> rel_19_new_InputOutputReceiveSend = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @new_InputOutputSend
std::unique_ptr<t_btree_2__0_1__1__3> rel_20_new_InputOutputSend = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @new_InputReceiveSend
std::unique_ptr<t_btree_2__0_1__1__3> rel_21_new_InputReceiveSend = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @new_InputSend
std::unique_ptr<t_btree_2__0_1__1__3> rel_22_new_InputSend = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @new_NegatedSender
std::unique_ptr<t_btree_2__0_1__3> rel_23_new_NegatedSender = std::make_unique<t_btree_2__0_1__3>();
// -- Table: @new_Output
std::unique_ptr<t_btree_2__0_1__1__3> rel_24_new_Output = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @new_OutputReceive
std::unique_ptr<t_btree_2__0_1__1__3> rel_25_new_OutputReceive = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @new_OutputReceiveSend
std::unique_ptr<t_btree_2__0_1__1__3> rel_26_new_OutputReceiveSend = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @new_OutputSend
std::unique_ptr<t_btree_2__0_1__1__3> rel_27_new_OutputSend = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @new_Receiver
std::unique_ptr<t_btree_2__0_1__1__3> rel_28_new_Receiver = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @new_Send
std::unique_ptr<t_btree_2__0_1__1__3> rel_29_new_Send = std::make_unique<t_btree_2__0_1__1__3>();
// -- Table: @new_Sender
std::unique_ptr<t_btree_2__0_1__3> rel_30_new_Sender = std::make_unique<t_btree_2__0_1__3>();
// -- Table: AggregatedSender
std::unique_ptr<t_btree_2__0_1__3> rel_31_AggregatedSender = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<0,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_31_AggregatedSender;
// -- Table: InputOutput
std::unique_ptr<t_btree_2__0_1__3> rel_32_InputOutput = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<1,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_32_InputOutput;
// -- Table: InputOutputReceive
std::unique_ptr<t_btree_2__0_1__3> rel_33_InputOutputReceive = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<2,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_33_InputOutputReceive;
// -- Table: InputOutputReceiveSend
std::unique_ptr<t_btree_2__0_1__3> rel_34_InputOutputReceiveSend = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<3,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_34_InputOutputReceiveSend;
// -- Table: InputOutputSend
std::unique_ptr<t_btree_2__0_1__3> rel_35_InputOutputSend = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<4,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_35_InputOutputSend;
// -- Table: InputReceiveSend
std::unique_ptr<t_btree_2__0_1__3> rel_36_InputReceiveSend = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<5,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_36_InputReceiveSend;
// -- Table: InputSend
std::unique_ptr<t_btree_2__0_1__3> rel_37_InputSend = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<6,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_37_InputSend;
// -- Table: NegatedSender
std::unique_ptr<t_btree_2__0_1__3> rel_38_NegatedSender = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<7,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_38_NegatedSender;
// -- Table: Output
std::unique_ptr<t_btree_2__0_1__3> rel_39_Output = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<8,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_39_Output;
// -- Table: OutputReceive
std::unique_ptr<t_btree_2__0_1__3> rel_40_OutputReceive = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<9,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_40_OutputReceive;
// -- Table: OutputReceiveSend
std::unique_ptr<t_btree_2__0_1__3> rel_41_OutputReceiveSend = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<10,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_41_OutputReceiveSend;
// -- Table: OutputSend
std::unique_ptr<t_btree_2__0_1__3> rel_42_OutputSend = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<11,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_42_OutputSend;
// -- Table: Receiver
std::unique_ptr<t_btree_2__0_1__3> rel_43_Receiver = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<12,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_43_Receiver;
// -- Table: Send
std::unique_ptr<t_btree_2__0_1__3> rel_44_Send = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<13,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_44_Send;
// -- Table: Sender
std::unique_ptr<t_btree_2__0_1__3> rel_45_Sender = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<14,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_45_Sender;
public:
Sf_foo() : 
wrapper_rel_31_AggregatedSender(*rel_31_AggregatedSender,symTable,"AggregatedSender",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_32_InputOutput(*rel_32_InputOutput,symTable,"InputOutput",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_33_InputOutputReceive(*rel_33_InputOutputReceive,symTable,"InputOutputReceive",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_34_InputOutputReceiveSend(*rel_34_InputOutputReceiveSend,symTable,"InputOutputReceiveSend",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_35_InputOutputSend(*rel_35_InputOutputSend,symTable,"InputOutputSend",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_36_InputReceiveSend(*rel_36_InputReceiveSend,symTable,"InputReceiveSend",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_37_InputSend(*rel_37_InputSend,symTable,"InputSend",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_38_NegatedSender(*rel_38_NegatedSender,symTable,"NegatedSender",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_39_Output(*rel_39_Output,symTable,"Output",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_40_OutputReceive(*rel_40_OutputReceive,symTable,"OutputReceive",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_41_OutputReceiveSend(*rel_41_OutputReceiveSend,symTable,"OutputReceiveSend",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_42_OutputSend(*rel_42_OutputSend,symTable,"OutputSend",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_43_Receiver(*rel_43_Receiver,symTable,"Receiver",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_44_Send(*rel_44_Send,symTable,"Send",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_45_Sender(*rel_45_Sender,symTable,"Sender",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}){
addRelation("AggregatedSender",&wrapper_rel_31_AggregatedSender,true,false);
addRelation("InputOutput",&wrapper_rel_32_InputOutput,true,false);
addRelation("InputOutputReceive",&wrapper_rel_33_InputOutputReceive,true,false);
addRelation("InputOutputReceiveSend",&wrapper_rel_34_InputOutputReceiveSend,true,false);
addRelation("InputOutputSend",&wrapper_rel_35_InputOutputSend,true,false);
addRelation("InputReceiveSend",&wrapper_rel_36_InputReceiveSend,true,false);
addRelation("InputSend",&wrapper_rel_37_InputSend,true,false);
addRelation("NegatedSender",&wrapper_rel_38_NegatedSender,true,false);
addRelation("Output",&wrapper_rel_39_Output,false,false);
addRelation("OutputReceive",&wrapper_rel_40_OutputReceive,false,false);
addRelation("OutputReceiveSend",&wrapper_rel_41_OutputReceiveSend,false,false);
addRelation("OutputSend",&wrapper_rel_42_OutputSend,false,false);
addRelation("Receiver",&wrapper_rel_43_Receiver,false,false);
addRelation("Send",&wrapper_rel_44_Send,false,false);
addRelation("Sender",&wrapper_rel_45_Sender,true,false);
}
~Sf_foo() {
}
private:
void runFunction(std::string inputDirectory = ".", std::string outputDirectory = ".", size_t stratumIndex = (size_t) -2, bool performIO = false) {
SignalHandler::instance()->set();
std::atomic<size_t> iter(0);

#if defined(_OPENMP)
if (getNumThreads() > 0) {omp_set_num_threads(getNumThreads());}
#endif

// -- query evaluation --
/* BEGIN STRATUM 0 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./AggregatedSender.facts"},{"location","fact-dir"},{"name","AggregatedSender"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_31_AggregatedSender);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_16_new_AggregatedSender->insertAll(*rel_31_AggregatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_1_delta_AggregatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_16_new_AggregatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 0 */
/* BEGIN STRATUM 1 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutput.facts"},{"location","fact-dir"},{"name","InputOutput"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_32_InputOutput);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_17_new_InputOutput->insertAll(*rel_32_InputOutput);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitLoop @Synthesiser.cpp:428 */
iter = 0;
for(;;) {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitParallel @Synthesiser.cpp:394 */
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_17_new_InputOutput_op_ctxt,rel_17_new_InputOutput->createContext());
CREATE_OP_CONTEXT(rel_32_InputOutput_op_ctxt,rel_32_InputOutput->createContext());
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_32_InputOutput->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
,/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_32_InputOutput_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
)}};
rel_17_new_InputOutput->insert(tuple,READ_OP_CONTEXT(rel_17_new_InputOutput_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitParallel @Synthesiser.cpp:406 */
/* BEGIN visitExit @Synthesiser.cpp:448 */
if(/* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_17_new_InputOutput->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
 && /* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_2_delta_InputOutput->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2147483647)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_2_delta_InputOutput_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) break;
/* END visitExit @Synthesiser.cpp:452 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_32_InputOutput->insertAll(*rel_17_new_InputOutput);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_2_delta_InputOutput, rel_17_new_InputOutput);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_17_new_InputOutput->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutput.csv"},{"location","output-dir"},{"name","InputOutput"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_2_delta_InputOutput);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
iter++;
}
iter = 0;
/* END visitLoop @Synthesiser.cpp:435 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_2_delta_InputOutput->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_17_new_InputOutput->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 1 */
/* BEGIN STRATUM 2 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./NegatedSender.facts"},{"location","fact-dir"},{"name","NegatedSender"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_38_NegatedSender);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_23_new_NegatedSender->insertAll(*rel_38_NegatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_8_delta_NegatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_23_new_NegatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 2 */
/* BEGIN STRATUM 3 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./Sender.facts"},{"location","fact-dir"},{"name","Sender"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_45_Sender);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_30_new_Sender->insertAll(*rel_45_Sender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_15_delta_Sender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_30_new_Sender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 3 */
/* BEGIN STRATUM 4 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutputReceive.facts"},{"location","fact-dir"},{"name","InputOutputReceive"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_33_InputOutputReceive);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_8_delta_NegatedSender->insertAll(*rel_38_NegatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_1_delta_AggregatedSender->insertAll(*rel_31_AggregatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_15_delta_Sender->insertAll(*rel_45_Sender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_18_new_InputOutputReceive->insertAll(*rel_33_InputOutputReceive);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitLoop @Synthesiser.cpp:428 */
iter = 0;
for(;;) {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitParallel @Synthesiser.cpp:394 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_18_new_InputOutputReceive_op_ctxt,rel_18_new_InputOutputReceive->createContext());
CREATE_OP_CONTEXT(rel_33_InputOutputReceive_op_ctxt,rel_33_InputOutputReceive->createContext());
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_33_InputOutputReceive->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
,/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_33_InputOutputReceive_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
)}};
rel_18_new_InputOutputReceive->insert(tuple,READ_OP_CONTEXT(rel_18_new_InputOutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceive(x,y) :- 
   Sender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [93:1-93:42])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_15_delta_Sender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_18_new_InputOutputReceive_op_ctxt,rel_18_new_InputOutputReceive->createContext());
CREATE_OP_CONTEXT(rel_33_InputOutputReceive_op_ctxt,rel_33_InputOutputReceive->createContext());
CREATE_OP_CONTEXT(rel_15_delta_Sender_op_ctxt,rel_15_delta_Sender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_15_delta_Sender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_33_InputOutputReceive->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_33_InputOutputReceive_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_18_new_InputOutputReceive->insert(tuple,READ_OP_CONTEXT(rel_18_new_InputOutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceive(x,y) :- 
   NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [94:1-94:49])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_8_delta_NegatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_18_new_InputOutputReceive_op_ctxt,rel_18_new_InputOutputReceive->createContext());
CREATE_OP_CONTEXT(rel_33_InputOutputReceive_op_ctxt,rel_33_InputOutputReceive->createContext());
CREATE_OP_CONTEXT(rel_8_delta_NegatedSender_op_ctxt,rel_8_delta_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_8_delta_NegatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_33_InputOutputReceive->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_33_InputOutputReceive_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_18_new_InputOutputReceive->insert(tuple,READ_OP_CONTEXT(rel_18_new_InputOutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceive(x,y) :- 
   AggregatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [95:1-95:52])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_1_delta_AggregatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_1_delta_AggregatedSender_op_ctxt,rel_1_delta_AggregatedSender->createContext());
CREATE_OP_CONTEXT(rel_18_new_InputOutputReceive_op_ctxt,rel_18_new_InputOutputReceive->createContext());
CREATE_OP_CONTEXT(rel_33_InputOutputReceive_op_ctxt,rel_33_InputOutputReceive->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_1_delta_AggregatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_33_InputOutputReceive->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_33_InputOutputReceive_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_18_new_InputOutputReceive->insert(tuple,READ_OP_CONTEXT(rel_18_new_InputOutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitParallel @Synthesiser.cpp:406 */
/* BEGIN visitExit @Synthesiser.cpp:448 */
if(/* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_18_new_InputOutputReceive->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
 && /* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_3_delta_InputOutputReceive->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2147483647)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_3_delta_InputOutputReceive_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) break;
/* END visitExit @Synthesiser.cpp:452 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_33_InputOutputReceive->insertAll(*rel_18_new_InputOutputReceive);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_3_delta_InputOutputReceive, rel_18_new_InputOutputReceive);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_18_new_InputOutputReceive->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutputReceive.csv"},{"location","output-dir"},{"name","InputOutputReceive"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_3_delta_InputOutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_38_NegatedSender->insertAll(*rel_23_new_NegatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_8_delta_NegatedSender, rel_23_new_NegatedSender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_23_new_NegatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_31_AggregatedSender->insertAll(*rel_16_new_AggregatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_1_delta_AggregatedSender, rel_16_new_AggregatedSender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_16_new_AggregatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_45_Sender->insertAll(*rel_30_new_Sender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_15_delta_Sender, rel_30_new_Sender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_30_new_Sender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
iter++;
}
iter = 0;
/* END visitLoop @Synthesiser.cpp:435 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_3_delta_InputOutputReceive->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_18_new_InputOutputReceive->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 4 */
/* BEGIN STRATUM 5 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutputReceiveSend.facts"},{"location","fact-dir"},{"name","InputOutputReceiveSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_34_InputOutputReceiveSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_8_delta_NegatedSender->insertAll(*rel_38_NegatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_1_delta_AggregatedSender->insertAll(*rel_31_AggregatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_15_delta_Sender->insertAll(*rel_45_Sender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_19_new_InputOutputReceiveSend->insertAll(*rel_34_InputOutputReceiveSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitLoop @Synthesiser.cpp:428 */
iter = 0;
for(;;) {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitParallel @Synthesiser.cpp:394 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_19_new_InputOutputReceiveSend_op_ctxt,rel_19_new_InputOutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_34_InputOutputReceiveSend_op_ctxt,rel_34_InputOutputReceiveSend->createContext());
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_34_InputOutputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
,/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_34_InputOutputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
)}};
rel_19_new_InputOutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_19_new_InputOutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceiveSend(x,y) :- 
   Sender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [133:1-133:46])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_15_delta_Sender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_19_new_InputOutputReceiveSend_op_ctxt,rel_19_new_InputOutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_34_InputOutputReceiveSend_op_ctxt,rel_34_InputOutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_15_delta_Sender_op_ctxt,rel_15_delta_Sender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_15_delta_Sender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_34_InputOutputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_34_InputOutputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_19_new_InputOutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_19_new_InputOutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceiveSend(x,y) :- 
   NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [134:1-134:53])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_8_delta_NegatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_19_new_InputOutputReceiveSend_op_ctxt,rel_19_new_InputOutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_34_InputOutputReceiveSend_op_ctxt,rel_34_InputOutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_8_delta_NegatedSender_op_ctxt,rel_8_delta_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_8_delta_NegatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_34_InputOutputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_34_InputOutputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_19_new_InputOutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_19_new_InputOutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceiveSend(x,y) :- 
   AggregatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [135:1-135:56])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_1_delta_AggregatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_1_delta_AggregatedSender_op_ctxt,rel_1_delta_AggregatedSender->createContext());
CREATE_OP_CONTEXT(rel_19_new_InputOutputReceiveSend_op_ctxt,rel_19_new_InputOutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_34_InputOutputReceiveSend_op_ctxt,rel_34_InputOutputReceiveSend->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_1_delta_AggregatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_34_InputOutputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_34_InputOutputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_19_new_InputOutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_19_new_InputOutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitParallel @Synthesiser.cpp:406 */
/* BEGIN visitExit @Synthesiser.cpp:448 */
if(/* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_19_new_InputOutputReceiveSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
 && /* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_4_delta_InputOutputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2147483647)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_4_delta_InputOutputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) break;
/* END visitExit @Synthesiser.cpp:452 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_34_InputOutputReceiveSend->insertAll(*rel_19_new_InputOutputReceiveSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_4_delta_InputOutputReceiveSend, rel_19_new_InputOutputReceiveSend);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_19_new_InputOutputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutputReceiveSend.csv"},{"location","output-dir"},{"name","InputOutputReceiveSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_4_delta_InputOutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_38_NegatedSender->insertAll(*rel_23_new_NegatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_8_delta_NegatedSender, rel_23_new_NegatedSender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_23_new_NegatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_31_AggregatedSender->insertAll(*rel_16_new_AggregatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_1_delta_AggregatedSender, rel_16_new_AggregatedSender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_16_new_AggregatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_45_Sender->insertAll(*rel_30_new_Sender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_15_delta_Sender, rel_30_new_Sender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_30_new_Sender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
iter++;
}
iter = 0;
/* END visitLoop @Synthesiser.cpp:435 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_4_delta_InputOutputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_19_new_InputOutputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 5 */
/* BEGIN STRATUM 7 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputReceiveSend.facts"},{"location","fact-dir"},{"name","InputReceiveSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_36_InputReceiveSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_8_delta_NegatedSender->insertAll(*rel_38_NegatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_1_delta_AggregatedSender->insertAll(*rel_31_AggregatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_15_delta_Sender->insertAll(*rel_45_Sender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_21_new_InputReceiveSend->insertAll(*rel_36_InputReceiveSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitLoop @Synthesiser.cpp:428 */
iter = 0;
for(;;) {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitParallel @Synthesiser.cpp:394 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_21_new_InputReceiveSend_op_ctxt,rel_21_new_InputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_36_InputReceiveSend_op_ctxt,rel_36_InputReceiveSend->createContext());
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_36_InputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
,/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_36_InputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
)}};
rel_21_new_InputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_21_new_InputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(InputReceiveSend(x,y) :- 
   Sender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [116:1-116:40])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_15_delta_Sender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_21_new_InputReceiveSend_op_ctxt,rel_21_new_InputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_36_InputReceiveSend_op_ctxt,rel_36_InputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_15_delta_Sender_op_ctxt,rel_15_delta_Sender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_15_delta_Sender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_36_InputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_36_InputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_21_new_InputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_21_new_InputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(InputReceiveSend(x,y) :- 
   NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [117:1-117:47])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_8_delta_NegatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_21_new_InputReceiveSend_op_ctxt,rel_21_new_InputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_36_InputReceiveSend_op_ctxt,rel_36_InputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_8_delta_NegatedSender_op_ctxt,rel_8_delta_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_8_delta_NegatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_36_InputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_36_InputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_21_new_InputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_21_new_InputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(InputReceiveSend(x,y) :- 
   AggregatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [118:1-118:50])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_1_delta_AggregatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_1_delta_AggregatedSender_op_ctxt,rel_1_delta_AggregatedSender->createContext());
CREATE_OP_CONTEXT(rel_21_new_InputReceiveSend_op_ctxt,rel_21_new_InputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_36_InputReceiveSend_op_ctxt,rel_36_InputReceiveSend->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_1_delta_AggregatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_36_InputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_36_InputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_21_new_InputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_21_new_InputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitParallel @Synthesiser.cpp:406 */
/* BEGIN visitExit @Synthesiser.cpp:448 */
if(/* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_21_new_InputReceiveSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
 && /* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_6_delta_InputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2147483647)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_6_delta_InputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) break;
/* END visitExit @Synthesiser.cpp:452 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_36_InputReceiveSend->insertAll(*rel_21_new_InputReceiveSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_6_delta_InputReceiveSend, rel_21_new_InputReceiveSend);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_21_new_InputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_38_NegatedSender->insertAll(*rel_23_new_NegatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_8_delta_NegatedSender, rel_23_new_NegatedSender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_23_new_NegatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_31_AggregatedSender->insertAll(*rel_16_new_AggregatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_1_delta_AggregatedSender, rel_16_new_AggregatedSender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_16_new_AggregatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_45_Sender->insertAll(*rel_30_new_Sender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_15_delta_Sender, rel_30_new_Sender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_30_new_Sender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
iter++;
}
iter = 0;
/* END visitLoop @Synthesiser.cpp:435 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_6_delta_InputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_21_new_InputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 7 */
/* BEGIN STRATUM 10 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_8_delta_NegatedSender->insertAll(*rel_38_NegatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_1_delta_AggregatedSender->insertAll(*rel_31_AggregatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_15_delta_Sender->insertAll(*rel_45_Sender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitLoop @Synthesiser.cpp:428 */
iter = 0;
for(;;) {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitParallel @Synthesiser.cpp:394 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_40_OutputReceive_op_ctxt,rel_40_OutputReceive->createContext());
CREATE_OP_CONTEXT(rel_25_new_OutputReceive_op_ctxt,rel_25_new_OutputReceive->createContext());
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_40_OutputReceive->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
,/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_40_OutputReceive_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
)}};
rel_25_new_OutputReceive->insert(tuple,READ_OP_CONTEXT(rel_25_new_OutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(OutputReceive(x,y) :- 
   Sender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [85:1-85:37])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_15_delta_Sender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_40_OutputReceive_op_ctxt,rel_40_OutputReceive->createContext());
CREATE_OP_CONTEXT(rel_25_new_OutputReceive_op_ctxt,rel_25_new_OutputReceive->createContext());
CREATE_OP_CONTEXT(rel_15_delta_Sender_op_ctxt,rel_15_delta_Sender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_15_delta_Sender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_40_OutputReceive->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_40_OutputReceive_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_25_new_OutputReceive->insert(tuple,READ_OP_CONTEXT(rel_25_new_OutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(OutputReceive(x,y) :- 
   NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [86:1-86:44])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_8_delta_NegatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_40_OutputReceive_op_ctxt,rel_40_OutputReceive->createContext());
CREATE_OP_CONTEXT(rel_25_new_OutputReceive_op_ctxt,rel_25_new_OutputReceive->createContext());
CREATE_OP_CONTEXT(rel_8_delta_NegatedSender_op_ctxt,rel_8_delta_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_8_delta_NegatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_40_OutputReceive->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_40_OutputReceive_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_25_new_OutputReceive->insert(tuple,READ_OP_CONTEXT(rel_25_new_OutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(OutputReceive(x,y) :- 
   AggregatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [87:1-87:47])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_1_delta_AggregatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_1_delta_AggregatedSender_op_ctxt,rel_1_delta_AggregatedSender->createContext());
CREATE_OP_CONTEXT(rel_40_OutputReceive_op_ctxt,rel_40_OutputReceive->createContext());
CREATE_OP_CONTEXT(rel_25_new_OutputReceive_op_ctxt,rel_25_new_OutputReceive->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_1_delta_AggregatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_40_OutputReceive->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_40_OutputReceive_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_25_new_OutputReceive->insert(tuple,READ_OP_CONTEXT(rel_25_new_OutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitParallel @Synthesiser.cpp:406 */
/* BEGIN visitExit @Synthesiser.cpp:448 */
if(/* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_25_new_OutputReceive->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
 && /* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_10_delta_OutputReceive->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2147483647)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_10_delta_OutputReceive_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) break;
/* END visitExit @Synthesiser.cpp:452 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_40_OutputReceive->insertAll(*rel_25_new_OutputReceive);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_10_delta_OutputReceive, rel_25_new_OutputReceive);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_25_new_OutputReceive->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./OutputReceive.csv"},{"location","output-dir"},{"name","OutputReceive"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_10_delta_OutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_38_NegatedSender->insertAll(*rel_23_new_NegatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_8_delta_NegatedSender, rel_23_new_NegatedSender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_23_new_NegatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_31_AggregatedSender->insertAll(*rel_16_new_AggregatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_1_delta_AggregatedSender, rel_16_new_AggregatedSender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_16_new_AggregatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_45_Sender->insertAll(*rel_30_new_Sender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_15_delta_Sender, rel_30_new_Sender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_30_new_Sender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
iter++;
}
iter = 0;
/* END visitLoop @Synthesiser.cpp:435 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_10_delta_OutputReceive->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_25_new_OutputReceive->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 10 */
/* BEGIN STRATUM 11 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_8_delta_NegatedSender->insertAll(*rel_38_NegatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_1_delta_AggregatedSender->insertAll(*rel_31_AggregatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_15_delta_Sender->insertAll(*rel_45_Sender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitLoop @Synthesiser.cpp:428 */
iter = 0;
for(;;) {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitParallel @Synthesiser.cpp:394 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_26_new_OutputReceiveSend_op_ctxt,rel_26_new_OutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_41_OutputReceiveSend_op_ctxt,rel_41_OutputReceiveSend->createContext());
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_41_OutputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
,/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_41_OutputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
)}};
rel_26_new_OutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_26_new_OutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(OutputReceiveSend(x,y) :- 
   Sender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [124:1-124:41])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_15_delta_Sender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_26_new_OutputReceiveSend_op_ctxt,rel_26_new_OutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_41_OutputReceiveSend_op_ctxt,rel_41_OutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_15_delta_Sender_op_ctxt,rel_15_delta_Sender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_15_delta_Sender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_41_OutputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_41_OutputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_26_new_OutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_26_new_OutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(OutputReceiveSend(x,y) :- 
   NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [125:1-125:48])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_8_delta_NegatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_26_new_OutputReceiveSend_op_ctxt,rel_26_new_OutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_41_OutputReceiveSend_op_ctxt,rel_41_OutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_8_delta_NegatedSender_op_ctxt,rel_8_delta_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_8_delta_NegatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_41_OutputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_41_OutputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_26_new_OutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_26_new_OutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(OutputReceiveSend(x,y) :- 
   AggregatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [126:1-126:51])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_1_delta_AggregatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_1_delta_AggregatedSender_op_ctxt,rel_1_delta_AggregatedSender->createContext());
CREATE_OP_CONTEXT(rel_26_new_OutputReceiveSend_op_ctxt,rel_26_new_OutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_41_OutputReceiveSend_op_ctxt,rel_41_OutputReceiveSend->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_1_delta_AggregatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_41_OutputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_41_OutputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_26_new_OutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_26_new_OutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitParallel @Synthesiser.cpp:406 */
/* BEGIN visitExit @Synthesiser.cpp:448 */
if(/* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_26_new_OutputReceiveSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
 && /* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_11_delta_OutputReceiveSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2147483647)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_11_delta_OutputReceiveSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) break;
/* END visitExit @Synthesiser.cpp:452 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_41_OutputReceiveSend->insertAll(*rel_26_new_OutputReceiveSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_11_delta_OutputReceiveSend, rel_26_new_OutputReceiveSend);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_26_new_OutputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./OutputReceiveSend.csv"},{"location","output-dir"},{"name","OutputReceiveSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_11_delta_OutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_38_NegatedSender->insertAll(*rel_23_new_NegatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_8_delta_NegatedSender, rel_23_new_NegatedSender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_23_new_NegatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_31_AggregatedSender->insertAll(*rel_16_new_AggregatedSender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_1_delta_AggregatedSender, rel_16_new_AggregatedSender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_16_new_AggregatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_45_Sender->insertAll(*rel_30_new_Sender);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_15_delta_Sender, rel_30_new_Sender);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_30_new_Sender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
iter++;
}
iter = 0;
/* END visitLoop @Synthesiser.cpp:435 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_11_delta_OutputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_26_new_OutputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 11 */
/* BEGIN STRATUM 6 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutputSend.facts"},{"location","fact-dir"},{"name","InputOutputSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_35_InputOutputSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_20_new_InputOutputSend->insertAll(*rel_35_InputOutputSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitLoop @Synthesiser.cpp:428 */
iter = 0;
for(;;) {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitParallel @Synthesiser.cpp:394 */
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_35_InputOutputSend_op_ctxt,rel_35_InputOutputSend->createContext());
CREATE_OP_CONTEXT(rel_20_new_InputOutputSend_op_ctxt,rel_20_new_InputOutputSend->createContext());
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_35_InputOutputSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
,/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_35_InputOutputSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
)}};
rel_20_new_InputOutputSend->insert(tuple,READ_OP_CONTEXT(rel_20_new_InputOutputSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitParallel @Synthesiser.cpp:406 */
/* BEGIN visitExit @Synthesiser.cpp:448 */
if(/* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_20_new_InputOutputSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
 && /* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_5_delta_InputOutputSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2147483647)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_5_delta_InputOutputSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) break;
/* END visitExit @Synthesiser.cpp:452 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_35_InputOutputSend->insertAll(*rel_20_new_InputOutputSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_5_delta_InputOutputSend, rel_20_new_InputOutputSend);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_20_new_InputOutputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutputSend.csv"},{"location","output-dir"},{"name","InputOutputSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_5_delta_InputOutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
iter++;
}
iter = 0;
/* END visitLoop @Synthesiser.cpp:435 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_5_delta_InputOutputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_20_new_InputOutputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 6 */
/* BEGIN STRATUM 8 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputSend.facts"},{"location","fact-dir"},{"name","InputSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_37_InputSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_22_new_InputSend->insertAll(*rel_37_InputSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitLoop @Synthesiser.cpp:428 */
iter = 0;
for(;;) {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitParallel @Synthesiser.cpp:394 */
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_37_InputSend_op_ctxt,rel_37_InputSend->createContext());
CREATE_OP_CONTEXT(rel_22_new_InputSend_op_ctxt,rel_22_new_InputSend->createContext());
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_37_InputSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
,/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_37_InputSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
)}};
rel_22_new_InputSend->insert(tuple,READ_OP_CONTEXT(rel_22_new_InputSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitParallel @Synthesiser.cpp:406 */
/* BEGIN visitExit @Synthesiser.cpp:448 */
if(/* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_22_new_InputSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
 && /* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_7_delta_InputSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2147483647)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_7_delta_InputSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) break;
/* END visitExit @Synthesiser.cpp:452 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_37_InputSend->insertAll(*rel_22_new_InputSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_7_delta_InputSend, rel_22_new_InputSend);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_22_new_InputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
iter++;
}
iter = 0;
/* END visitLoop @Synthesiser.cpp:435 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_7_delta_InputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_22_new_InputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()&& performIO) rel_45_Sender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 8 */
/* BEGIN STRATUM 9 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitLoop @Synthesiser.cpp:428 */
iter = 0;
for(;;) {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitParallel @Synthesiser.cpp:394 */
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_39_Output_op_ctxt,rel_39_Output->createContext());
CREATE_OP_CONTEXT(rel_24_new_Output_op_ctxt,rel_24_new_Output->createContext());
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_39_Output->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
,/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_39_Output_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
)}};
rel_24_new_Output->insert(tuple,READ_OP_CONTEXT(rel_24_new_Output_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitParallel @Synthesiser.cpp:406 */
/* BEGIN visitExit @Synthesiser.cpp:448 */
if(/* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_24_new_Output->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
 && /* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_9_delta_Output->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2147483647)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_9_delta_Output_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) break;
/* END visitExit @Synthesiser.cpp:452 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_39_Output->insertAll(*rel_24_new_Output);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_9_delta_Output, rel_24_new_Output);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_24_new_Output->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./Output.csv"},{"location","output-dir"},{"name","Output"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_9_delta_Output);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
iter++;
}
iter = 0;
/* END visitLoop @Synthesiser.cpp:435 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_9_delta_Output->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_24_new_Output->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 9 */
/* BEGIN STRATUM 12 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitLoop @Synthesiser.cpp:428 */
iter = 0;
for(;;) {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitParallel @Synthesiser.cpp:394 */
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_27_new_OutputSend_op_ctxt,rel_27_new_OutputSend->createContext());
CREATE_OP_CONTEXT(rel_42_OutputSend_op_ctxt,rel_42_OutputSend->createContext());
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_42_OutputSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
,/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_42_OutputSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
)}};
rel_27_new_OutputSend->insert(tuple,READ_OP_CONTEXT(rel_27_new_OutputSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitParallel @Synthesiser.cpp:406 */
/* BEGIN visitExit @Synthesiser.cpp:448 */
if(/* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_27_new_OutputSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
 && /* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_12_delta_OutputSend->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2147483647)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_12_delta_OutputSend_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) break;
/* END visitExit @Synthesiser.cpp:452 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_42_OutputSend->insertAll(*rel_27_new_OutputSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_12_delta_OutputSend, rel_27_new_OutputSend);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_27_new_OutputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./OutputSend.csv"},{"location","output-dir"},{"name","OutputSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_12_delta_OutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
iter++;
}
iter = 0;
/* END visitLoop @Synthesiser.cpp:435 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_12_delta_OutputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_27_new_OutputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 12 */
/* BEGIN STRATUM 13 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitLoop @Synthesiser.cpp:428 */
iter = 0;
for(;;) {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitParallel @Synthesiser.cpp:394 */
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_29_new_Send_op_ctxt,rel_29_new_Send->createContext());
CREATE_OP_CONTEXT(rel_44_Send_op_ctxt,rel_44_Send->createContext());
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_44_Send->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
,/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_44_Send_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1341 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1341 */
)}};
rel_29_new_Send->insert(tuple,READ_OP_CONTEXT(rel_29_new_Send_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitParallel @Synthesiser.cpp:406 */
/* BEGIN visitExit @Synthesiser.cpp:448 */
if(/* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_29_new_Send->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
 && /* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_14_delta_Send->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2147483647)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_14_delta_Send_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) break;
/* END visitExit @Synthesiser.cpp:452 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_44_Send->insertAll(*rel_29_new_Send);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_14_delta_Send, rel_29_new_Send);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_29_new_Send->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
iter++;
}
iter = 0;
/* END visitLoop @Synthesiser.cpp:435 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_14_delta_Send->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_29_new_Send->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 13 */
/* BEGIN STRATUM 14 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_14_delta_Send->insertAll(*rel_44_Send);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_7_delta_InputSend->insertAll(*rel_37_InputSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_12_delta_OutputSend->insertAll(*rel_42_OutputSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_5_delta_InputOutputSend->insertAll(*rel_35_InputOutputSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_6_delta_InputReceiveSend->insertAll(*rel_36_InputReceiveSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_11_delta_OutputReceiveSend->insertAll(*rel_41_OutputReceiveSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_4_delta_InputOutputReceiveSend->insertAll(*rel_34_InputOutputReceiveSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitLoop @Synthesiser.cpp:428 */
iter = 0;
for(;;) {
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitParallel @Synthesiser.cpp:394 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   Send(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [67:1-67:52])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_14_delta_Send->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_43_Receiver_op_ctxt,rel_43_Receiver->createContext());
CREATE_OP_CONTEXT(rel_14_delta_Send_op_ctxt,rel_14_delta_Send->createContext());
CREATE_OP_CONTEXT(rel_38_NegatedSender_op_ctxt,rel_38_NegatedSender->createContext());
CREATE_OP_CONTEXT(rel_28_new_Receiver_op_ctxt,rel_28_new_Receiver->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_14_delta_Send) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_38_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_38_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
 && /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_43_Receiver->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_43_Receiver_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_28_new_Receiver->insert(tuple,READ_OP_CONTEXT(rel_28_new_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   InputSend(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [100:1-100:57])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_7_delta_InputSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_7_delta_InputSend_op_ctxt,rel_7_delta_InputSend->createContext());
CREATE_OP_CONTEXT(rel_43_Receiver_op_ctxt,rel_43_Receiver->createContext());
CREATE_OP_CONTEXT(rel_38_NegatedSender_op_ctxt,rel_38_NegatedSender->createContext());
CREATE_OP_CONTEXT(rel_28_new_Receiver_op_ctxt,rel_28_new_Receiver->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_7_delta_InputSend) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_38_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_38_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
 && /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_43_Receiver->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_43_Receiver_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_28_new_Receiver->insert(tuple,READ_OP_CONTEXT(rel_28_new_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   OutputSend(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [105:1-105:58])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_12_delta_OutputSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_43_Receiver_op_ctxt,rel_43_Receiver->createContext());
CREATE_OP_CONTEXT(rel_12_delta_OutputSend_op_ctxt,rel_12_delta_OutputSend->createContext());
CREATE_OP_CONTEXT(rel_38_NegatedSender_op_ctxt,rel_38_NegatedSender->createContext());
CREATE_OP_CONTEXT(rel_28_new_Receiver_op_ctxt,rel_28_new_Receiver->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_12_delta_OutputSend) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_38_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_38_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
 && /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_43_Receiver->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_43_Receiver_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_28_new_Receiver->insert(tuple,READ_OP_CONTEXT(rel_28_new_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   InputOutputSend(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [111:1-111:63])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_5_delta_InputOutputSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_43_Receiver_op_ctxt,rel_43_Receiver->createContext());
CREATE_OP_CONTEXT(rel_5_delta_InputOutputSend_op_ctxt,rel_5_delta_InputOutputSend->createContext());
CREATE_OP_CONTEXT(rel_38_NegatedSender_op_ctxt,rel_38_NegatedSender->createContext());
CREATE_OP_CONTEXT(rel_28_new_Receiver_op_ctxt,rel_28_new_Receiver->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_5_delta_InputOutputSend) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_38_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_38_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
 && /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_43_Receiver->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_43_Receiver_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_28_new_Receiver->insert(tuple,READ_OP_CONTEXT(rel_28_new_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   InputReceiveSend(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [119:1-119:64])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_6_delta_InputReceiveSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_6_delta_InputReceiveSend_op_ctxt,rel_6_delta_InputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_43_Receiver_op_ctxt,rel_43_Receiver->createContext());
CREATE_OP_CONTEXT(rel_38_NegatedSender_op_ctxt,rel_38_NegatedSender->createContext());
CREATE_OP_CONTEXT(rel_28_new_Receiver_op_ctxt,rel_28_new_Receiver->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_6_delta_InputReceiveSend) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_38_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_38_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
 && /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_43_Receiver->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_43_Receiver_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_28_new_Receiver->insert(tuple,READ_OP_CONTEXT(rel_28_new_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   OutputReceiveSend(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [127:1-127:65])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_11_delta_OutputReceiveSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_11_delta_OutputReceiveSend_op_ctxt,rel_11_delta_OutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_43_Receiver_op_ctxt,rel_43_Receiver->createContext());
CREATE_OP_CONTEXT(rel_38_NegatedSender_op_ctxt,rel_38_NegatedSender->createContext());
CREATE_OP_CONTEXT(rel_28_new_Receiver_op_ctxt,rel_28_new_Receiver->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_11_delta_OutputReceiveSend) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_38_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_38_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
 && /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_43_Receiver->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_43_Receiver_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_28_new_Receiver->insert(tuple,READ_OP_CONTEXT(rel_28_new_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:493 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   InputOutputReceiveSend(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [136:1-136:70])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_4_delta_InputOutputReceiveSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
[&](){
CREATE_OP_CONTEXT(rel_43_Receiver_op_ctxt,rel_43_Receiver->createContext());
CREATE_OP_CONTEXT(rel_4_delta_InputOutputReceiveSend_op_ctxt,rel_4_delta_InputOutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_38_NegatedSender_op_ctxt,rel_38_NegatedSender->createContext());
CREATE_OP_CONTEXT(rel_28_new_Receiver_op_ctxt,rel_28_new_Receiver->createContext());
/* BEGIN visitScan @Synthesiser.cpp:557 */
for(const auto& env0 : *rel_4_delta_InputOutputReceiveSend) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_38_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_38_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
 && /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_43_Receiver->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_43_Receiver_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_28_new_Receiver->insert(tuple,READ_OP_CONTEXT(rel_28_new_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitScan @Synthesiser.cpp:568 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:500 */
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_31_AggregatedSender_op_ctxt,rel_31_AggregatedSender->createContext());
CREATE_OP_CONTEXT(rel_43_Receiver_op_ctxt,rel_43_Receiver->createContext());
CREATE_OP_CONTEXT(rel_28_new_Receiver_op_ctxt,rel_28_new_Receiver->createContext());
/* BEGIN visitAggregate @Synthesiser.cpp:973 */
ram::Tuple<RamDomain,1> env0;
RamDomain res0 = MAX_RAM_DOMAIN;
for(const auto& env0 : *rel_31_AggregatedSender) {
if( /* BEGIN visitTrue @Synthesiser.cpp:1110 */
true/* END visitTrue @Synthesiser.cpp:1112 */
) {
res0 = std::min(res0,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
);
}
}
env0[0] = res0;
if(res0 != MAX_RAM_DOMAIN){
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitAggregate @Synthesiser.cpp:973 */
ram::Tuple<RamDomain,1> env1;
RamDomain res1 = MIN_RAM_DOMAIN;
for(const auto& env1 : *rel_31_AggregatedSender) {
if( /* BEGIN visitTrue @Synthesiser.cpp:1110 */
true/* END visitTrue @Synthesiser.cpp:1112 */
) {
res1 = std::max(res1,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env1[1]/* END visitTupleElement @Synthesiser.cpp:1347 */
);
}
}
env1[0] = res1;
if(res1 != MIN_RAM_DOMAIN){
/* BEGIN visitTupleOperation @Synthesiser.cpp:518 */
/* BEGIN visitFilter @Synthesiser.cpp:1067 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1130 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_43_Receiver->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env1[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
}},READ_OP_CONTEXT(rel_43_Receiver_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
)/* END visitNegation @Synthesiser.cpp:1134 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1086 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1345 */
env1[0]/* END visitTupleElement @Synthesiser.cpp:1347 */
)}};
rel_28_new_Receiver->insert(tuple,READ_OP_CONTEXT(rel_28_new_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1104 */
}
/* END visitFilter @Synthesiser.cpp:1073 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitAggregate @Synthesiser.cpp:1063 */
/* END visitTupleOperation @Synthesiser.cpp:520 */
}
/* END visitAggregate @Synthesiser.cpp:1063 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitParallel @Synthesiser.cpp:406 */
/* BEGIN visitExit @Synthesiser.cpp:448 */
if(/* BEGIN visitConjunction @Synthesiser.cpp:1122 */
/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1225 */
rel_28_new_Receiver->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1228 */
 && /* BEGIN visitExistenceCheck @Synthesiser.cpp:1232 */
rel_13_delta_Receiver->contains(Tuple<RamDomain,2>{{/* BEGIN visitNumber @Synthesiser.cpp:1339 */
RamDomain(2147483647)/* END visitNumber @Synthesiser.cpp:1341 */
}},READ_OP_CONTEXT(rel_13_delta_Receiver_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1250 */
/* END visitConjunction @Synthesiser.cpp:1126 */
) break;
/* END visitExit @Synthesiser.cpp:452 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_43_Receiver->insertAll(*rel_28_new_Receiver);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_13_delta_Receiver, rel_28_new_Receiver);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_28_new_Receiver->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./Receiver.csv"},{"location","output-dir"},{"name","Receiver"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_13_delta_Receiver);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_44_Send->insertAll(*rel_29_new_Send);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_14_delta_Send, rel_29_new_Send);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_29_new_Send->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_37_InputSend->insertAll(*rel_22_new_InputSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_7_delta_InputSend, rel_22_new_InputSend);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_22_new_InputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_42_OutputSend->insertAll(*rel_27_new_OutputSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_12_delta_OutputSend, rel_27_new_OutputSend);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_27_new_OutputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_35_InputOutputSend->insertAll(*rel_20_new_InputOutputSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_5_delta_InputOutputSend, rel_20_new_InputOutputSend);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_20_new_InputOutputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_36_InputReceiveSend->insertAll(*rel_21_new_InputReceiveSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_6_delta_InputReceiveSend, rel_21_new_InputReceiveSend);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_21_new_InputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_41_OutputReceiveSend->insertAll(*rel_26_new_OutputReceiveSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_11_delta_OutputReceiveSend, rel_26_new_OutputReceiveSend);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_26_new_OutputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitMerge @Synthesiser.cpp:352 */
rel_34_InputOutputReceiveSend->insertAll(*rel_19_new_InputOutputReceiveSend);
/* END visitMerge @Synthesiser.cpp:361 */
/* BEGIN visitSwap @Synthesiser.cpp:439 */
std::swap(rel_4_delta_InputOutputReceiveSend, rel_19_new_InputOutputReceiveSend);
/* END visitSwap @Synthesiser.cpp:444 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_19_new_InputOutputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
iter++;
}
iter = 0;
/* END visitLoop @Synthesiser.cpp:435 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitSequence @Synthesiser.cpp:386 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_13_delta_Receiver->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()) rel_28_new_Receiver->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
/* END visitSequence @Synthesiser.cpp:390 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()&& performIO) rel_38_NegatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()&& performIO) rel_31_AggregatedSender->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()&& performIO) rel_44_Send->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()&& performIO) rel_37_InputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()&& performIO) rel_42_OutputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()&& performIO) rel_35_InputOutputSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()&& performIO) rel_36_InputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()&& performIO) rel_41_OutputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* BEGIN visitClear @Synthesiser.cpp:365 */
if (!isHintsProfilingEnabled()&& performIO) rel_34_InputOutputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:372 */
/* END visitSequence @Synthesiser.cpp:390 */
}();
/* END STRATUM 14 */

// -- relation hint statistics --
if(isHintsProfilingEnabled()) {
std::cout << " -- Operation Hint Statistics --\n";
std::cout << "Relation rel_1_delta_AggregatedSender:\n";
rel_1_delta_AggregatedSender->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_2_delta_InputOutput:\n";
rel_2_delta_InputOutput->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_3_delta_InputOutputReceive:\n";
rel_3_delta_InputOutputReceive->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_4_delta_InputOutputReceiveSend:\n";
rel_4_delta_InputOutputReceiveSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_5_delta_InputOutputSend:\n";
rel_5_delta_InputOutputSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_6_delta_InputReceiveSend:\n";
rel_6_delta_InputReceiveSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_7_delta_InputSend:\n";
rel_7_delta_InputSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_8_delta_NegatedSender:\n";
rel_8_delta_NegatedSender->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_9_delta_Output:\n";
rel_9_delta_Output->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_10_delta_OutputReceive:\n";
rel_10_delta_OutputReceive->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_11_delta_OutputReceiveSend:\n";
rel_11_delta_OutputReceiveSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_12_delta_OutputSend:\n";
rel_12_delta_OutputSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_13_delta_Receiver:\n";
rel_13_delta_Receiver->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_14_delta_Send:\n";
rel_14_delta_Send->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_15_delta_Sender:\n";
rel_15_delta_Sender->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_16_new_AggregatedSender:\n";
rel_16_new_AggregatedSender->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_17_new_InputOutput:\n";
rel_17_new_InputOutput->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_18_new_InputOutputReceive:\n";
rel_18_new_InputOutputReceive->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_19_new_InputOutputReceiveSend:\n";
rel_19_new_InputOutputReceiveSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_20_new_InputOutputSend:\n";
rel_20_new_InputOutputSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_21_new_InputReceiveSend:\n";
rel_21_new_InputReceiveSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_22_new_InputSend:\n";
rel_22_new_InputSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_23_new_NegatedSender:\n";
rel_23_new_NegatedSender->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_24_new_Output:\n";
rel_24_new_Output->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_25_new_OutputReceive:\n";
rel_25_new_OutputReceive->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_26_new_OutputReceiveSend:\n";
rel_26_new_OutputReceiveSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_27_new_OutputSend:\n";
rel_27_new_OutputSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_28_new_Receiver:\n";
rel_28_new_Receiver->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_29_new_Send:\n";
rel_29_new_Send->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_30_new_Sender:\n";
rel_30_new_Sender->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_31_AggregatedSender:\n";
rel_31_AggregatedSender->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_32_InputOutput:\n";
rel_32_InputOutput->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_33_InputOutputReceive:\n";
rel_33_InputOutputReceive->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_34_InputOutputReceiveSend:\n";
rel_34_InputOutputReceiveSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_35_InputOutputSend:\n";
rel_35_InputOutputSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_36_InputReceiveSend:\n";
rel_36_InputReceiveSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_37_InputSend:\n";
rel_37_InputSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_38_NegatedSender:\n";
rel_38_NegatedSender->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_39_Output:\n";
rel_39_Output->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_40_OutputReceive:\n";
rel_40_OutputReceive->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_41_OutputReceiveSend:\n";
rel_41_OutputReceiveSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_42_OutputSend:\n";
rel_42_OutputSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_43_Receiver:\n";
rel_43_Receiver->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_44_Send:\n";
rel_44_Send->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_45_Sender:\n";
rel_45_Sender->printHintStatistics(std::cout,"  ");
std::cout << "\n";
}
SignalHandler::instance()->reset();
}
public:
void run(size_t stratumIndex = (size_t) -2) override { runFunction(".", ".", stratumIndex, false); }
public:
void runAll(std::string inputDirectory = ".", std::string outputDirectory = ".", size_t stratumIndex = (size_t) -2) override { runFunction(inputDirectory, outputDirectory, stratumIndex, true);
}
public:
void printAll(std::string outputDirectory = ".") override {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutput.csv"},{"location","output-dir"},{"name","InputOutput"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_2_delta_InputOutput);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutputReceive.csv"},{"location","output-dir"},{"name","InputOutputReceive"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_3_delta_InputOutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutputReceiveSend.csv"},{"location","output-dir"},{"name","InputOutputReceiveSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_4_delta_InputOutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./OutputReceive.csv"},{"location","output-dir"},{"name","OutputReceive"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_10_delta_OutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./OutputReceiveSend.csv"},{"location","output-dir"},{"name","OutputReceiveSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_11_delta_OutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutputSend.csv"},{"location","output-dir"},{"name","InputOutputSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_5_delta_InputOutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./Output.csv"},{"location","output-dir"},{"name","Output"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_9_delta_Output);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./OutputSend.csv"},{"location","output-dir"},{"name","OutputSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_12_delta_OutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","true"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./Receiver.csv"},{"location","output-dir"},{"name","Receiver"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_13_delta_Receiver);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
public:
void loadAll(std::string inputDirectory = ".") override {
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./AggregatedSender.facts"},{"location","fact-dir"},{"name","AggregatedSender"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_31_AggregatedSender);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutput.facts"},{"location","fact-dir"},{"name","InputOutput"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_32_InputOutput);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./NegatedSender.facts"},{"location","fact-dir"},{"name","NegatedSender"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_38_NegatedSender);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./Sender.facts"},{"location","fact-dir"},{"name","Sender"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_45_Sender);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutputReceive.facts"},{"location","fact-dir"},{"name","InputOutputReceive"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_33_InputOutputReceive);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutputReceiveSend.facts"},{"location","fact-dir"},{"name","InputOutputReceiveSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_34_InputOutputReceiveSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputReceiveSend.facts"},{"location","fact-dir"},{"name","InputReceiveSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_36_InputReceiveSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutputSend.facts"},{"location","fact-dir"},{"name","InputOutputSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_35_InputOutputSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"",""},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputSend.facts"},{"location","fact-dir"},{"name","InputSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_37_InputSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
public:
void dumpInputs(std::ostream& out = std::cout) override {
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("AggregatedSender");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_31_AggregatedSender);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputOutput");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_32_InputOutput);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("NegatedSender");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_38_NegatedSender);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("Sender");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_45_Sender);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputOutputReceive");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_33_InputOutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputOutputReceiveSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_34_InputOutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputReceiveSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_36_InputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputOutputSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_35_InputOutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_37_InputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
public:
void dumpOutputs(std::ostream& out = std::cout) override {
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("@delta_InputOutput");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_2_delta_InputOutput);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("@delta_InputOutputReceive");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_3_delta_InputOutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("@delta_InputOutputReceiveSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_4_delta_InputOutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("@delta_OutputReceive");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_10_delta_OutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("@delta_OutputReceiveSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_11_delta_OutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("@delta_InputOutputSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_5_delta_InputOutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("@delta_Output");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_9_delta_Output);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("@delta_OutputSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_12_delta_OutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("@delta_Receiver");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_13_delta_Receiver);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
public:
SymbolTable& getSymbolTable() override {
return symTable;
}
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
static factory_Sf_foo __factory_Sf_foo_instance;
}
#else
}
int main(int argc, char** argv)
{
try{
souffle::CmdOptions opt(R"(tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl)",
R"(.)",
R"(.)",
false,
R"()",
1);
if (!opt.parse(argc,argv)) return 1;
souffle::Sf_foo obj;
#if defined(_OPENMP) 
obj.setNumThreads(opt.getNumJobs());

#endif
obj.runAll(opt.getInputFileDir(), opt.getOutputFileDir(), opt.getStratumIndex());
return 0;
} catch(std::exception &e) { souffle::SignalHandler::instance()->error(e.what());}
}

#endif
