#ifndef CYCLUS_EXCHANGE_CONTEXT_H_
#define CYCLUS_EXCHANGE_CONTEXT_H_

#include <assert.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "bid.h"
#include "bid_portfolio.h"
#include "request.h"
#include "request_portfolio.h"

namespace cyclus {

template <class T>
struct PrefMap {
  typedef std::map< typename Request<T>::Ptr,
    std::vector< std::pair< typename Bid<T>::Ptr, double > > > type;
};

/// @class ExchangeContext
///
/// @brief The ExchangeContext is designed to provide an ease-of-use interface
/// for querying and reaggregating information regarding requests and bids of a
/// resource exchange.
///
/// The ExchangeContext is used by a ResourceExchange or related class to
/// provide introspection into the requests and bids it collects. Specifically,
/// this class is designed to assist in phases of the Dynamic Resource
/// Exchange. The second phase, Respose to Request for Bids, is assisted by
/// grouping requests by commodity type. The third phase, preference adjustment,
/// is assisted by grouping bids by the requester being responded to.
template <class T>
struct ExchangeContext {
 public:  
  /// @brief adds a request to the context
  void AddRequestPortfolio(const typename RequestPortfolio<T>::Ptr port) {
    requests.push_back(port);
    const std::vector<typename Request<T>::Ptr>& vr = port->requests();
    typename std::vector<typename Request<T>::Ptr>::const_iterator it;
    
    for (it = vr.begin(); it != vr.end(); ++it) {
      typename Request<T>::Ptr pr = *it;
      AddRequest(*it);
    }
  }

  /// @brief Adds an individual request
  void AddRequest(typename Request<T>::Ptr pr) {
    assert(pr->requester() != NULL);
    requesters.insert(pr->requester());
    requests_by_commod[pr->commodity()].push_back(pr);
  }
  
  /// @brief adds a bid to the context
  void AddBidPortfolio(const typename BidPortfolio<T>::Ptr port) {
    bids.push_back(port);
    const std::set<typename Bid<T>::Ptr>& vr = port->bids();
    typename std::set<typename Bid<T>::Ptr>::const_iterator it;
    
    for (it = vr.begin(); it != vr.end(); ++it) {
      typename Bid<T>::Ptr pb = *it;
      AddBid(pb);
    }
  }

  /// @brief adds a bid to the appropriate containers
  /// @param pb the bid
  void AddBid(typename Bid<T>::Ptr pb) {
    assert(pb->bidder() != NULL);
    bidders.insert(pb->bidder());
    
    bids_by_request[pb->request()].push_back(pb);

    trader_prefs[pb->request()->requester()][pb->request()].push_back(
        std::make_pair(pb, pb->request()->preference()));
  }

  /* /// @return all known request portfolios */
  /* inline const std::vector<typename RequestPortfolio<T>::Ptr>& */
  /*     requests() const { */
  /*   return requests_; */
  /* } */
  
  /* /// @return all known requesters */
  /* inline const std::set<Trader*>& requesters() const { return requesters_; } */

  /* /// @return all known bid portfolios */
  /* inline const std::vector<typename BidPortfolio<T>::Ptr>& */
  /*     bids() const { */
  /*   return bids_; */
  /* } */

  /* /// @return all known bidders */
  /* inline const std::set<Trader*>& bidders() const { return bidders_; } */
  
  /* /// @return all known requests for a given commodity */
  /* /// @param commod the commodity */
  /* inline const std::vector<typename Request<T>::Ptr>& */
  /*     requests_by_commod[std]::string commod) const { */
  /*   return requests_by_commod.at(commod); */
  /* } */

  /* inline const std::vector<typename Request<T>::Ptr>& */
  /*     requests_by_commod[std]::string commod) { */
  /*   return requests_by_commod[commod]; */
  /* } */
  
  /* /// @return all known bids for a request */
  /* /// @param request the request */
  /* inline const std::vector<typename Bid<T>::Ptr>& */
  /*     BidsForRequest(typename Request<T>::Ptr request) const { */
  /*   return bids_by_request.at(request); */
  /* } */

  /* inline const std::vector<typename Bid<T>::Ptr>& */
  /*     BidsForRequest(typename Request<T>::Ptr request) { */
  /*   return bids_by_request[request]; */
  /* } */

  /* /// @return all known preferences for a requester */
  /* /// @param requester the requester */
  /* inline typename PrefMap<T>::type& Prefs(Trader* requester) const { */
  /*   return trader_prefs.at(requester); */
  /* } */

  /* inline typename PrefMap<T>::type& Prefs(Trader* requester) { */
  /*   return trader_prefs[requester]; */
  /* } */
  
  /// @brief a reference to an exchange's set of requests
  std::vector<typename RequestPortfolio<T>::Ptr> requests;

  /// @brief a reference to an exchange's set of bids
  std::vector<typename BidPortfolio<T>::Ptr> bids;

  /// @brief known requesters
  std::set<Trader*> requesters;
  
  /// @brief known bidders
  std::set<Trader*> bidders;
  
  /// @brief maps commodity name to requests for that commodity
  std::map< std::string, std::vector<typename Request<T>::Ptr> >
      requests_by_commod;

  /// @brief maps request to all bids for request
  std::map< typename Request<T>::Ptr, std::vector<typename Bid<T>::Ptr> >
      bids_by_request;

  /// @brief maps commodity name to requests for that commodity
  std::map<Trader*, typename PrefMap<T>::type> trader_prefs; 
};

} // namespace cyclus

#endif // #ifndef CYCLUS_EXCHANGE_CONTEXT_H_
