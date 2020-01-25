
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
;// -- Table: AggregatedSender
std::unique_ptr<t_btree_2__0_1__3> rel_1_AggregatedSender = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<0,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_1_AggregatedSender;
// -- Table: InputOutput
std::unique_ptr<t_btree_2__0_1__3> rel_2_InputOutput = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<1,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_2_InputOutput;
// -- Table: InputOutputReceive
std::unique_ptr<t_btree_2__0_1__3> rel_3_InputOutputReceive = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<2,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_3_InputOutputReceive;
// -- Table: InputOutputReceiveSend
std::unique_ptr<t_btree_2__0_1__3> rel_4_InputOutputReceiveSend = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<3,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_4_InputOutputReceiveSend;
// -- Table: InputOutputSend
std::unique_ptr<t_btree_2__0_1__3> rel_5_InputOutputSend = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<4,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_5_InputOutputSend;
// -- Table: InputReceiveSend
std::unique_ptr<t_btree_2__0_1__3> rel_6_InputReceiveSend = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<5,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_6_InputReceiveSend;
// -- Table: InputSend
std::unique_ptr<t_btree_2__0_1__3> rel_7_InputSend = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<6,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_7_InputSend;
// -- Table: NegatedSender
std::unique_ptr<t_btree_2__0_1__3> rel_8_NegatedSender = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<7,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_8_NegatedSender;
// -- Table: Output
std::unique_ptr<t_btree_2__0_1__3> rel_9_Output = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<8,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_9_Output;
// -- Table: OutputReceive
std::unique_ptr<t_btree_2__0_1__3> rel_10_OutputReceive = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<9,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_10_OutputReceive;
// -- Table: OutputReceiveSend
std::unique_ptr<t_btree_2__0_1__3> rel_11_OutputReceiveSend = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<10,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_11_OutputReceiveSend;
// -- Table: OutputSend
std::unique_ptr<t_btree_2__0_1__3> rel_12_OutputSend = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<11,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_12_OutputSend;
// -- Table: Receiver
std::unique_ptr<t_btree_2__0_1__3> rel_13_Receiver = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<12,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_13_Receiver;
// -- Table: Send
std::unique_ptr<t_btree_2__0_1__3> rel_14_Send = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<13,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_14_Send;
// -- Table: Sender
std::unique_ptr<t_btree_2__0_1__3> rel_15_Sender = std::make_unique<t_btree_2__0_1__3>();
souffle::RelationWrapper<14,t_btree_2__0_1__3,Tuple<RamDomain,2>,2,1> wrapper_rel_15_Sender;
public:
Sf_foo() : 
wrapper_rel_1_AggregatedSender(*rel_1_AggregatedSender,symTable,"AggregatedSender",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_2_InputOutput(*rel_2_InputOutput,symTable,"InputOutput",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_3_InputOutputReceive(*rel_3_InputOutputReceive,symTable,"InputOutputReceive",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_4_InputOutputReceiveSend(*rel_4_InputOutputReceiveSend,symTable,"InputOutputReceiveSend",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_5_InputOutputSend(*rel_5_InputOutputSend,symTable,"InputOutputSend",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_6_InputReceiveSend(*rel_6_InputReceiveSend,symTable,"InputReceiveSend",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_7_InputSend(*rel_7_InputSend,symTable,"InputSend",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_8_NegatedSender(*rel_8_NegatedSender,symTable,"NegatedSender",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_9_Output(*rel_9_Output,symTable,"Output",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_10_OutputReceive(*rel_10_OutputReceive,symTable,"OutputReceive",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_11_OutputReceiveSend(*rel_11_OutputReceiveSend,symTable,"OutputReceiveSend",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_12_OutputSend(*rel_12_OutputSend,symTable,"OutputSend",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_13_Receiver(*rel_13_Receiver,symTable,"Receiver",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_14_Send(*rel_14_Send,symTable,"Send",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}),

wrapper_rel_15_Sender(*rel_15_Sender,symTable,"Sender",std::array<const char *,2>{{"i:number","i:number"}},std::array<const char *,2>{{"x","y"}}){
addRelation("AggregatedSender",&wrapper_rel_1_AggregatedSender,true,false);
addRelation("InputOutput",&wrapper_rel_2_InputOutput,true,true);
addRelation("InputOutputReceive",&wrapper_rel_3_InputOutputReceive,true,true);
addRelation("InputOutputReceiveSend",&wrapper_rel_4_InputOutputReceiveSend,true,true);
addRelation("InputOutputSend",&wrapper_rel_5_InputOutputSend,true,true);
addRelation("InputReceiveSend",&wrapper_rel_6_InputReceiveSend,true,false);
addRelation("InputSend",&wrapper_rel_7_InputSend,true,false);
addRelation("NegatedSender",&wrapper_rel_8_NegatedSender,true,false);
addRelation("Output",&wrapper_rel_9_Output,false,true);
addRelation("OutputReceive",&wrapper_rel_10_OutputReceive,false,true);
addRelation("OutputReceiveSend",&wrapper_rel_11_OutputReceiveSend,false,true);
addRelation("OutputSend",&wrapper_rel_12_OutputSend,false,true);
addRelation("Receiver",&wrapper_rel_13_Receiver,false,true);
addRelation("Send",&wrapper_rel_14_Send,false,false);
addRelation("Sender",&wrapper_rel_15_Sender,true,false);
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
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./AggregatedSender.facts"},{"location","fact-dir"},{"name","AggregatedSender"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_1_AggregatedSender);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
}();
/* END STRATUM 0 */
/* BEGIN STRATUM 1 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutput.facts"},{"location","fact-dir"},{"name","InputOutput"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_2_InputOutput);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputOutput(1,2).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [57:1-57:19])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_2_InputOutput_op_ctxt,rel_2_InputOutput->createContext());
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1357 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1357 */
)}};
rel_2_InputOutput->insert(tuple,READ_OP_CONTEXT(rel_2_InputOutput_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutput.csv"},{"location","output-dir"},{"name","InputOutput"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_2_InputOutput);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:377 */
}();
/* END STRATUM 1 */
/* BEGIN STRATUM 2 */
[&]() {
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./NegatedSender.facts"},{"location","fact-dir"},{"name","NegatedSender"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_8_NegatedSender);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
}();
/* END STRATUM 2 */
/* BEGIN STRATUM 3 */
[&]() {
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./Sender.facts"},{"location","fact-dir"},{"name","Sender"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_15_Sender);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
}();
/* END STRATUM 3 */
/* BEGIN STRATUM 4 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutputReceive.facts"},{"location","fact-dir"},{"name","InputOutputReceive"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_3_InputOutputReceive);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceive(1,2).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [92:1-92:26])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_3_InputOutputReceive_op_ctxt,rel_3_InputOutputReceive->createContext());
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1357 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1357 */
)}};
rel_3_InputOutputReceive->insert(tuple,READ_OP_CONTEXT(rel_3_InputOutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceive(x,y) :- 
   Sender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [93:1-93:42])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_15_Sender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_3_InputOutputReceive_op_ctxt,rel_3_InputOutputReceive->createContext());
CREATE_OP_CONTEXT(rel_15_Sender_op_ctxt,rel_15_Sender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_15_Sender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_3_InputOutputReceive->insert(tuple,READ_OP_CONTEXT(rel_3_InputOutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceive(x,y) :- 
   NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [94:1-94:49])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_8_NegatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_3_InputOutputReceive_op_ctxt,rel_3_InputOutputReceive->createContext());
CREATE_OP_CONTEXT(rel_8_NegatedSender_op_ctxt,rel_8_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_8_NegatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_3_InputOutputReceive->insert(tuple,READ_OP_CONTEXT(rel_3_InputOutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceive(x,y) :- 
   AggregatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [95:1-95:52])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_1_AggregatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_1_AggregatedSender_op_ctxt,rel_1_AggregatedSender->createContext());
CREATE_OP_CONTEXT(rel_3_InputOutputReceive_op_ctxt,rel_3_InputOutputReceive->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_1_AggregatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_3_InputOutputReceive->insert(tuple,READ_OP_CONTEXT(rel_3_InputOutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* END visitSequence @Synthesiser.cpp:377 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutputReceive.csv"},{"location","output-dir"},{"name","InputOutputReceive"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_3_InputOutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:377 */
}();
/* END STRATUM 4 */
/* BEGIN STRATUM 5 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutputReceiveSend.facts"},{"location","fact-dir"},{"name","InputOutputReceiveSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_4_InputOutputReceiveSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceiveSend(1,2).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [132:1-132:30])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_4_InputOutputReceiveSend_op_ctxt,rel_4_InputOutputReceiveSend->createContext());
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1357 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1357 */
)}};
rel_4_InputOutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_4_InputOutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceiveSend(x,y) :- 
   Sender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [133:1-133:46])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_15_Sender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_4_InputOutputReceiveSend_op_ctxt,rel_4_InputOutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_15_Sender_op_ctxt,rel_15_Sender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_15_Sender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_4_InputOutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_4_InputOutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceiveSend(x,y) :- 
   NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [134:1-134:53])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_8_NegatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_4_InputOutputReceiveSend_op_ctxt,rel_4_InputOutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_8_NegatedSender_op_ctxt,rel_8_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_8_NegatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_4_InputOutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_4_InputOutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputOutputReceiveSend(x,y) :- 
   AggregatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [135:1-135:56])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_1_AggregatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_4_InputOutputReceiveSend_op_ctxt,rel_4_InputOutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_1_AggregatedSender_op_ctxt,rel_1_AggregatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_1_AggregatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_4_InputOutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_4_InputOutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* END visitSequence @Synthesiser.cpp:377 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutputReceiveSend.csv"},{"location","output-dir"},{"name","InputOutputReceiveSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_4_InputOutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:377 */
}();
/* END STRATUM 5 */
/* BEGIN STRATUM 7 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputReceiveSend.facts"},{"location","fact-dir"},{"name","InputReceiveSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_6_InputReceiveSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputReceiveSend(1,2).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [115:1-115:24])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_6_InputReceiveSend_op_ctxt,rel_6_InputReceiveSend->createContext());
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1357 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1357 */
)}};
rel_6_InputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_6_InputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputReceiveSend(x,y) :- 
   Sender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [116:1-116:40])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_15_Sender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_6_InputReceiveSend_op_ctxt,rel_6_InputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_15_Sender_op_ctxt,rel_15_Sender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_15_Sender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_6_InputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_6_InputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputReceiveSend(x,y) :- 
   NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [117:1-117:47])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_8_NegatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_6_InputReceiveSend_op_ctxt,rel_6_InputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_8_NegatedSender_op_ctxt,rel_8_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_8_NegatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_6_InputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_6_InputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputReceiveSend(x,y) :- 
   AggregatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [118:1-118:50])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_1_AggregatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_1_AggregatedSender_op_ctxt,rel_1_AggregatedSender->createContext());
CREATE_OP_CONTEXT(rel_6_InputReceiveSend_op_ctxt,rel_6_InputReceiveSend->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_1_AggregatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_6_InputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_6_InputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* END visitSequence @Synthesiser.cpp:377 */
/* END visitSequence @Synthesiser.cpp:377 */
}();
/* END STRATUM 7 */
/* BEGIN STRATUM 10 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(OutputReceive(1,2).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [84:1-84:21])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_10_OutputReceive_op_ctxt,rel_10_OutputReceive->createContext());
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1357 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1357 */
)}};
rel_10_OutputReceive->insert(tuple,READ_OP_CONTEXT(rel_10_OutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(OutputReceive(x,y) :- 
   Sender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [85:1-85:37])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_15_Sender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_10_OutputReceive_op_ctxt,rel_10_OutputReceive->createContext());
CREATE_OP_CONTEXT(rel_15_Sender_op_ctxt,rel_15_Sender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_15_Sender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_10_OutputReceive->insert(tuple,READ_OP_CONTEXT(rel_10_OutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(OutputReceive(x,y) :- 
   NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [86:1-86:44])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_8_NegatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_10_OutputReceive_op_ctxt,rel_10_OutputReceive->createContext());
CREATE_OP_CONTEXT(rel_8_NegatedSender_op_ctxt,rel_8_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_8_NegatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_10_OutputReceive->insert(tuple,READ_OP_CONTEXT(rel_10_OutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(OutputReceive(x,y) :- 
   AggregatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [87:1-87:47])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_1_AggregatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_1_AggregatedSender_op_ctxt,rel_1_AggregatedSender->createContext());
CREATE_OP_CONTEXT(rel_10_OutputReceive_op_ctxt,rel_10_OutputReceive->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_1_AggregatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_10_OutputReceive->insert(tuple,READ_OP_CONTEXT(rel_10_OutputReceive_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./OutputReceive.csv"},{"location","output-dir"},{"name","OutputReceive"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_10_OutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:377 */
}();
/* END STRATUM 10 */
/* BEGIN STRATUM 11 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(OutputReceiveSend(1,2).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [123:1-123:25])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_11_OutputReceiveSend_op_ctxt,rel_11_OutputReceiveSend->createContext());
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1357 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1357 */
)}};
rel_11_OutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_11_OutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(OutputReceiveSend(x,y) :- 
   Sender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [124:1-124:41])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_15_Sender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_11_OutputReceiveSend_op_ctxt,rel_11_OutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_15_Sender_op_ctxt,rel_15_Sender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_15_Sender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_11_OutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_11_OutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(OutputReceiveSend(x,y) :- 
   NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [125:1-125:48])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_8_NegatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_11_OutputReceiveSend_op_ctxt,rel_11_OutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_8_NegatedSender_op_ctxt,rel_8_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_8_NegatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_11_OutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_11_OutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(OutputReceiveSend(x,y) :- 
   AggregatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [126:1-126:51])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_1_AggregatedSender->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_1_AggregatedSender_op_ctxt,rel_1_AggregatedSender->createContext());
CREATE_OP_CONTEXT(rel_11_OutputReceiveSend_op_ctxt,rel_11_OutputReceiveSend->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_1_AggregatedSender) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_11_OutputReceiveSend->insert(tuple,READ_OP_CONTEXT(rel_11_OutputReceiveSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./OutputReceiveSend.csv"},{"location","output-dir"},{"name","OutputReceiveSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_11_OutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:377 */
}();
/* END STRATUM 11 */
/* BEGIN STRATUM 6 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutputSend.facts"},{"location","fact-dir"},{"name","InputOutputSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_5_InputOutputSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputOutputSend(1,2).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [110:1-110:23])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_5_InputOutputSend_op_ctxt,rel_5_InputOutputSend->createContext());
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1357 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1357 */
)}};
rel_5_InputOutputSend->insert(tuple,READ_OP_CONTEXT(rel_5_InputOutputSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutputSend.csv"},{"location","output-dir"},{"name","InputOutputSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_5_InputOutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:377 */
}();
/* END STRATUM 6 */
/* BEGIN STRATUM 8 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitLoad @Synthesiser.cpp:204 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputSend.facts"},{"location","fact-dir"},{"name","InputSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_7_InputSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
/* END visitLoad @Synthesiser.cpp:231 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(InputSend(1,2).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [99:1-99:17])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_7_InputSend_op_ctxt,rel_7_InputSend->createContext());
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1357 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1357 */
)}};
rel_7_InputSend->insert(tuple,READ_OP_CONTEXT(rel_7_InputSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitClear @Synthesiser.cpp:352 */
if (!isHintsProfilingEnabled()&& performIO) rel_15_Sender->purge();
/* END visitClear @Synthesiser.cpp:359 */
/* END visitSequence @Synthesiser.cpp:377 */
}();
/* END STRATUM 8 */
/* BEGIN STRATUM 9 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(Output(1,2).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [52:1-52:14])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_9_Output_op_ctxt,rel_9_Output->createContext());
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1357 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1357 */
)}};
rel_9_Output->insert(tuple,READ_OP_CONTEXT(rel_9_Output_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./Output.csv"},{"location","output-dir"},{"name","Output"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_9_Output);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:377 */
}();
/* END STRATUM 9 */
/* BEGIN STRATUM 12 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(OutputSend(1,2).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [104:1-104:18])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_12_OutputSend_op_ctxt,rel_12_OutputSend->createContext());
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1357 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1357 */
)}};
rel_12_OutputSend->insert(tuple,READ_OP_CONTEXT(rel_12_OutputSend_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./OutputSend.csv"},{"location","output-dir"},{"name","OutputSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_12_OutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* END visitSequence @Synthesiser.cpp:377 */
}();
/* END STRATUM 12 */
/* BEGIN STRATUM 13 */
[&]() {
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(Send(1,2).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [66:1-66:12])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_14_Send_op_ctxt,rel_14_Send->createContext());
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(1)/* END visitNumber @Synthesiser.cpp:1357 */
),static_cast<RamDomain>(/* BEGIN visitNumber @Synthesiser.cpp:1355 */
RamDomain(2)/* END visitNumber @Synthesiser.cpp:1357 */
)}};
rel_14_Send->insert(tuple,READ_OP_CONTEXT(rel_14_Send_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
}();
/* END STRATUM 13 */
/* BEGIN STRATUM 14 */
[&]() {
/* BEGIN visitSequence @Synthesiser.cpp:373 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   Send(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [67:1-67:52])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_14_Send->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_14_Send_op_ctxt,rel_14_Send->createContext());
CREATE_OP_CONTEXT(rel_13_Receiver_op_ctxt,rel_13_Receiver->createContext());
CREATE_OP_CONTEXT(rel_8_NegatedSender_op_ctxt,rel_8_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_14_Send) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitFilter @Synthesiser.cpp:1083 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1248 */
rel_8_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
}},READ_OP_CONTEXT(rel_8_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1266 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_13_Receiver->insert(tuple,READ_OP_CONTEXT(rel_13_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
/* END visitFilter @Synthesiser.cpp:1089 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   InputSend(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [100:1-100:57])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_7_InputSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_13_Receiver_op_ctxt,rel_13_Receiver->createContext());
CREATE_OP_CONTEXT(rel_7_InputSend_op_ctxt,rel_7_InputSend->createContext());
CREATE_OP_CONTEXT(rel_8_NegatedSender_op_ctxt,rel_8_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_7_InputSend) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitFilter @Synthesiser.cpp:1083 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1248 */
rel_8_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
}},READ_OP_CONTEXT(rel_8_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1266 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_13_Receiver->insert(tuple,READ_OP_CONTEXT(rel_13_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
/* END visitFilter @Synthesiser.cpp:1089 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   OutputSend(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [105:1-105:58])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_12_OutputSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_13_Receiver_op_ctxt,rel_13_Receiver->createContext());
CREATE_OP_CONTEXT(rel_12_OutputSend_op_ctxt,rel_12_OutputSend->createContext());
CREATE_OP_CONTEXT(rel_8_NegatedSender_op_ctxt,rel_8_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_12_OutputSend) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitFilter @Synthesiser.cpp:1083 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1248 */
rel_8_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
}},READ_OP_CONTEXT(rel_8_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1266 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_13_Receiver->insert(tuple,READ_OP_CONTEXT(rel_13_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
/* END visitFilter @Synthesiser.cpp:1089 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   InputOutputSend(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [111:1-111:63])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_5_InputOutputSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_5_InputOutputSend_op_ctxt,rel_5_InputOutputSend->createContext());
CREATE_OP_CONTEXT(rel_13_Receiver_op_ctxt,rel_13_Receiver->createContext());
CREATE_OP_CONTEXT(rel_8_NegatedSender_op_ctxt,rel_8_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_5_InputOutputSend) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitFilter @Synthesiser.cpp:1083 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1248 */
rel_8_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
}},READ_OP_CONTEXT(rel_8_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1266 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_13_Receiver->insert(tuple,READ_OP_CONTEXT(rel_13_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
/* END visitFilter @Synthesiser.cpp:1089 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   InputReceiveSend(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [119:1-119:64])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_6_InputReceiveSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_6_InputReceiveSend_op_ctxt,rel_6_InputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_13_Receiver_op_ctxt,rel_13_Receiver->createContext());
CREATE_OP_CONTEXT(rel_8_NegatedSender_op_ctxt,rel_8_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_6_InputReceiveSend) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitFilter @Synthesiser.cpp:1083 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1248 */
rel_8_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
}},READ_OP_CONTEXT(rel_8_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1266 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_13_Receiver->insert(tuple,READ_OP_CONTEXT(rel_13_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
/* END visitFilter @Synthesiser.cpp:1089 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   OutputReceiveSend(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [127:1-127:65])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_11_OutputReceiveSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_11_OutputReceiveSend_op_ctxt,rel_11_OutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_13_Receiver_op_ctxt,rel_13_Receiver->createContext());
CREATE_OP_CONTEXT(rel_8_NegatedSender_op_ctxt,rel_8_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_11_OutputReceiveSend) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitFilter @Synthesiser.cpp:1083 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1248 */
rel_8_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
}},READ_OP_CONTEXT(rel_8_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1266 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_13_Receiver->insert(tuple,READ_OP_CONTEXT(rel_13_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
/* END visitFilter @Synthesiser.cpp:1089 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(Receiver(x,y) :- 
   InputOutputReceiveSend(x,y),
   !NegatedSender(x,y).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [136:1-136:70])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
if(/* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitEmptinessCheck @Synthesiser.cpp:1241 */
rel_4_InputOutputReceiveSend->empty()/* END visitEmptinessCheck @Synthesiser.cpp:1244 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
[&](){
CREATE_OP_CONTEXT(rel_4_InputOutputReceiveSend_op_ctxt,rel_4_InputOutputReceiveSend->createContext());
CREATE_OP_CONTEXT(rel_13_Receiver_op_ctxt,rel_13_Receiver->createContext());
CREATE_OP_CONTEXT(rel_8_NegatedSender_op_ctxt,rel_8_NegatedSender->createContext());
/* BEGIN visitScan @Synthesiser.cpp:573 */
for(const auto& env0 : *rel_4_InputOutputReceiveSend) {
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitFilter @Synthesiser.cpp:1083 */
if( /* BEGIN visitNegation @Synthesiser.cpp:1146 */
!(/* BEGIN visitExistenceCheck @Synthesiser.cpp:1248 */
rel_8_NegatedSender->contains(Tuple<RamDomain,2>{{/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
,/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
}},READ_OP_CONTEXT(rel_8_NegatedSender_op_ctxt))/* END visitExistenceCheck @Synthesiser.cpp:1266 */
)/* END visitNegation @Synthesiser.cpp:1150 */
) {
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_13_Receiver->insert(tuple,READ_OP_CONTEXT(rel_13_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
}
/* END visitFilter @Synthesiser.cpp:1089 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitScan @Synthesiser.cpp:584 */
}
();}
/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitDebugInfo @Synthesiser.cpp:509 */
SignalHandler::instance()->setMsg(R"_(Receiver(min  z0 : AggregatedSender( z0,_),max  z1 : AggregatedSender(_, z1)).
in file /home/lyndonhenry/souffle/tests/testsuite.dir/example/input_output_numbers/input_output_numbers.dl [44:1-44:90])_");
/* BEGIN visitQuery @Synthesiser.cpp:262 */
[&](){
CREATE_OP_CONTEXT(rel_1_AggregatedSender_op_ctxt,rel_1_AggregatedSender->createContext());
CREATE_OP_CONTEXT(rel_13_Receiver_op_ctxt,rel_13_Receiver->createContext());
/* BEGIN visitAggregate @Synthesiser.cpp:989 */
ram::Tuple<RamDomain,1> env0;
RamDomain res0 = MAX_RAM_DOMAIN;
for(const auto& env0 : *rel_1_AggregatedSender) {
if( /* BEGIN visitTrue @Synthesiser.cpp:1126 */
true/* END visitTrue @Synthesiser.cpp:1128 */
) {
res0 = std::min(res0,/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
);
}
}
env0[0] = res0;
if(res0 != MAX_RAM_DOMAIN){
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitAggregate @Synthesiser.cpp:989 */
ram::Tuple<RamDomain,1> env1;
RamDomain res1 = MIN_RAM_DOMAIN;
for(const auto& env1 : *rel_1_AggregatedSender) {
if( /* BEGIN visitTrue @Synthesiser.cpp:1126 */
true/* END visitTrue @Synthesiser.cpp:1128 */
) {
res1 = std::max(res1,/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env1[1]/* END visitTupleElement @Synthesiser.cpp:1363 */
);
}
}
env1[0] = res1;
if(res1 != MIN_RAM_DOMAIN){
/* BEGIN visitTupleOperation @Synthesiser.cpp:534 */
/* BEGIN visitProject @Synthesiser.cpp:1102 */
Tuple<RamDomain,2> tuple{{static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env0[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
),static_cast<RamDomain>(/* BEGIN visitTupleElement @Synthesiser.cpp:1361 */
env1[0]/* END visitTupleElement @Synthesiser.cpp:1363 */
)}};
rel_13_Receiver->insert(tuple,READ_OP_CONTEXT(rel_13_Receiver_op_ctxt));
/* END visitProject @Synthesiser.cpp:1120 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitAggregate @Synthesiser.cpp:1079 */
/* END visitTupleOperation @Synthesiser.cpp:536 */
}
/* END visitAggregate @Synthesiser.cpp:1079 */
}
();/* END visitQuery @Synthesiser.cpp:348 */
/* END visitDebugInfo @Synthesiser.cpp:516 */
/* BEGIN visitStore @Synthesiser.cpp:235 */
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./Receiver.csv"},{"location","output-dir"},{"name","Receiver"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_13_Receiver);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
/* END visitStore @Synthesiser.cpp:258 */
/* BEGIN visitClear @Synthesiser.cpp:352 */
if (!isHintsProfilingEnabled()&& performIO) rel_8_NegatedSender->purge();
/* END visitClear @Synthesiser.cpp:359 */
/* BEGIN visitClear @Synthesiser.cpp:352 */
if (!isHintsProfilingEnabled()&& performIO) rel_1_AggregatedSender->purge();
/* END visitClear @Synthesiser.cpp:359 */
/* BEGIN visitClear @Synthesiser.cpp:352 */
if (!isHintsProfilingEnabled()&& performIO) rel_14_Send->purge();
/* END visitClear @Synthesiser.cpp:359 */
/* BEGIN visitClear @Synthesiser.cpp:352 */
if (!isHintsProfilingEnabled()&& performIO) rel_7_InputSend->purge();
/* END visitClear @Synthesiser.cpp:359 */
/* BEGIN visitClear @Synthesiser.cpp:352 */
if (!isHintsProfilingEnabled()&& performIO) rel_12_OutputSend->purge();
/* END visitClear @Synthesiser.cpp:359 */
/* BEGIN visitClear @Synthesiser.cpp:352 */
if (!isHintsProfilingEnabled()&& performIO) rel_5_InputOutputSend->purge();
/* END visitClear @Synthesiser.cpp:359 */
/* BEGIN visitClear @Synthesiser.cpp:352 */
if (!isHintsProfilingEnabled()&& performIO) rel_6_InputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:359 */
/* BEGIN visitClear @Synthesiser.cpp:352 */
if (!isHintsProfilingEnabled()&& performIO) rel_11_OutputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:359 */
/* BEGIN visitClear @Synthesiser.cpp:352 */
if (!isHintsProfilingEnabled()&& performIO) rel_4_InputOutputReceiveSend->purge();
/* END visitClear @Synthesiser.cpp:359 */
/* END visitSequence @Synthesiser.cpp:377 */
}();
/* END STRATUM 14 */

// -- relation hint statistics --
if(isHintsProfilingEnabled()) {
std::cout << " -- Operation Hint Statistics --\n";
std::cout << "Relation rel_1_AggregatedSender:\n";
rel_1_AggregatedSender->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_2_InputOutput:\n";
rel_2_InputOutput->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_3_InputOutputReceive:\n";
rel_3_InputOutputReceive->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_4_InputOutputReceiveSend:\n";
rel_4_InputOutputReceiveSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_5_InputOutputSend:\n";
rel_5_InputOutputSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_6_InputReceiveSend:\n";
rel_6_InputReceiveSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_7_InputSend:\n";
rel_7_InputSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_8_NegatedSender:\n";
rel_8_NegatedSender->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_9_Output:\n";
rel_9_Output->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_10_OutputReceive:\n";
rel_10_OutputReceive->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_11_OutputReceiveSend:\n";
rel_11_OutputReceiveSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_12_OutputSend:\n";
rel_12_OutputSend->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_13_Receiver:\n";
rel_13_Receiver->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_14_Send:\n";
rel_14_Send->printHintStatistics(std::cout,"  ");
std::cout << "\n";
std::cout << "Relation rel_15_Sender:\n";
rel_15_Sender->printHintStatistics(std::cout,"  ");
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
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutput.csv"},{"location","output-dir"},{"name","InputOutput"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_2_InputOutput);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutputReceive.csv"},{"location","output-dir"},{"name","InputOutputReceive"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_3_InputOutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutputReceiveSend.csv"},{"location","output-dir"},{"name","InputOutputReceiveSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_4_InputOutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./OutputReceive.csv"},{"location","output-dir"},{"name","OutputReceive"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_10_OutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./OutputReceiveSend.csv"},{"location","output-dir"},{"name","OutputReceiveSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_11_OutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./InputOutputSend.csv"},{"location","output-dir"},{"name","InputOutputSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_5_InputOutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./Output.csv"},{"location","output-dir"},{"name","Output"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_9_Output);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./OutputSend.csv"},{"location","output-dir"},{"name","OutputSend"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_12_OutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"append","false"},{"attributeNames","x\ty"},{"directory","."},{"extension",".csv"},{"filename","./Receiver.csv"},{"location","output-dir"},{"name","Receiver"},{"stratum","slave"}});
if (!outputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = outputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_13_Receiver);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
public:
void loadAll(std::string inputDirectory = ".") override {
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./AggregatedSender.facts"},{"location","fact-dir"},{"name","AggregatedSender"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_1_AggregatedSender);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutput.facts"},{"location","fact-dir"},{"name","InputOutput"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_2_InputOutput);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./NegatedSender.facts"},{"location","fact-dir"},{"name","NegatedSender"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_8_NegatedSender);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./Sender.facts"},{"location","fact-dir"},{"name","Sender"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_15_Sender);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutputReceive.facts"},{"location","fact-dir"},{"name","InputOutputReceive"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_3_InputOutputReceive);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutputReceiveSend.facts"},{"location","fact-dir"},{"name","InputOutputReceiveSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_4_InputOutputReceiveSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputReceiveSend.facts"},{"location","fact-dir"},{"name","InputReceiveSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_6_InputReceiveSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputOutputSend.facts"},{"location","fact-dir"},{"name","InputOutputSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_5_InputOutputSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
try {std::map<std::string, std::string> directiveMap({{"","default"},{"IO","file"},{"directory","."},{"extension",".facts"},{"filename","./InputSend.facts"},{"location","fact-dir"},{"name","InputSend"},{"stratum","slave"}});
if (!inputDirectory.empty() && directiveMap["IO"] == "file" && directiveMap["filename"].front() != '/') {directiveMap["filename"] = inputDirectory + "/" + directiveMap["filename"];}
IODirectives ioDirectives(directiveMap);
IOSystem::getInstance().getReader(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->readAll(*rel_7_InputSend);
} catch (std::exception& e) {std::cerr << "Error loading data: " << e.what() << '\n';}
}
public:
void dumpInputs(std::ostream& out = std::cout) override {
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("AggregatedSender");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_1_AggregatedSender);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputOutput");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_2_InputOutput);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("NegatedSender");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_8_NegatedSender);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("Sender");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_15_Sender);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputOutputReceive");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_3_InputOutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputOutputReceiveSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_4_InputOutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputReceiveSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_6_InputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputOutputSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_5_InputOutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_7_InputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
public:
void dumpOutputs(std::ostream& out = std::cout) override {
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputOutput");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_2_InputOutput);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputOutputReceive");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_3_InputOutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputOutputReceiveSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_4_InputOutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("OutputReceive");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_10_OutputReceive);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("OutputReceiveSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_11_OutputReceiveSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("InputOutputSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_5_InputOutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("Output");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_9_Output);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("OutputSend");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_12_OutputSend);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
try {IODirectives ioDirectives;
ioDirectives.setIOType("stdout");
ioDirectives.setRelationName("Receiver");
IOSystem::getInstance().getWriter(std::vector<bool>({0,0}), symTable, ioDirectives, false, 1)->writeAll(*rel_13_Receiver);
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
