#ifndef CYCLUS_PROG_TRANSLATOR_H_
#define CYCLUS_PROG_TRANSLATOR_H_

#include <vector>

#include "CoinPackedMatrix.hpp"

class OsiSolverInterface;

namespace cyclus {

class ExchangeGraph;
class ExchangeNodeGroup;

/// a helper class to translate a generic resource exchange into a mathematical
/// program.
///
/// use as follows:
/// @code
/// // given a graph, g, and interface, iface
/// ProgTranslator t(g, iface);
/// t->ToProg();
/// iface->Solve(); // or whatever member function
/// t->FromProg();
/// @endcode
class ProgTranslator {
 public:
  /// @brief struct to hold all problem instance state
  struct Context {
    std::vector<double> obj_coeffs;
    std::vector<double> row_ubs;
    std::vector<double> row_lbs;
    std::vector<double> col_ubs;
    std::vector<double> col_lbs;
    CoinPackedMatrix m;    
  };
  
  /// constructor
  ///
  /// @param g the exchange graph
  /// @param iface the solver interface
  /// @param exclusive whether or not to include binary-valued arcs
  ProgTranslator(ExchangeGraph* g, OsiSolverInterface* iface,
                 bool exclusive = false);

  /// @brief translates graph into mathematic program via iface
  void ToProg();

  /// @brief translates solution from iface back into graph matches
  void FromProg();

  const ProgTranslator::Context& ctx() const { return ctx_; }
  
 private:
  /// perform all translation for a node group
  /// @param grp a pointer to the node group
  /// @param req a boolean flag, true if grp is a request group
  void XlateGrp_(ExchangeNodeGroup* grp, bool req);
  
  ExchangeGraph* g_;
  OsiSolverInterface* iface_;
  bool excl_;
  int arc_offset_;
  ProgTranslator::Context ctx_;
};

} // namespace cyclus

#endif // ifndef CYCLUS_PROG_TRANSLATOR_H_

