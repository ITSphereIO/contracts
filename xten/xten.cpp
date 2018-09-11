#include "xten.hpp"

void xten::createorder(
   const uint64_t       id,
   const account_name   creator
) {
   require_auth( _self );

   eosio::action(
      eosio::permission_level{ _self, N(active) },
      N(eosio.token), N(transfer),
      std::make_tuple(_self, creator, eosio::asset(2500000, S(4, ITS)), std::string(""))
   ).send();

   orders.emplace(_self, [&](auto& _order) {
      _order.id         = id;
      _order.creator    = creator;
   });
}

void xten::createoffer(
   const uint64_t       id,
   const uint64_t       order_id,
   const account_name   contractor
) {
   require_auth( contractor );

   auto order = orders.find(order_id);

   eosio_assert(order != orders.end(), "Order does not exist");
   eosio_assert(order->creator != contractor, "Can not respond to your orders");

   eosio::action(
      eosio::permission_level{ contractor, N(active) },
      N(eosio.token), N(transfer),
      std::make_tuple(contractor, _self, eosio::asset(1000000, S(4, ITS)), std::string(""))
   ).send();

   offers.emplace(_self, [&](auto& _offer) {
      _offer.id            = id;
      _offer.order_id      = order_id;
      _offer.contractor    = contractor;
   });
}

void xten::createdoc(
   const uint64_t       id,
   const uint64_t       offer_id
) {
   auto offer = offers.find(offer_id);

   eosio_assert(offer != offers.end(), "Offer does not exist");

   require_auth( offer->contractor );

   documents.emplace(_self, [&](auto& _document) {
      _document.id         = id;
      _document.offer_id   = offer_id;
   });
}

EOSIO_ABI(xten, (createorder)(createoffer)(createdoc))