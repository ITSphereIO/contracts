#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

class xten : public eosio::contract {
   public:
      xten(account_name self) : eosio::contract(self), orders(_self, _self), offers(_self, _self), documents(_self, _self) {}

      // @abi action
      void createorder(
         uint64_t       id,
         account_name   creator
      );
      // @abi action
      void createoffer(
         uint64_t       id,
         uint64_t       order_id,
         account_name   contractor
      );
      // @abi action
      void createdoc(
         uint64_t       id,
         uint64_t       offer_id
      );

   private:
      // @abi table orders
      struct order {
         uint64_t       id;
         account_name   creator;

         uint64_t primary_key() const { return id; }

         EOSLIB_SERIALIZE(order, (id)(creator))
      };
      // @abi table offers
      struct offer {
         uint64_t       id;
         uint64_t       order_id;
         account_name   contractor;

         uint64_t primary_key() const { return id; }

         EOSLIB_SERIALIZE(offer, (id)(order_id)(contractor))
      };
      // @abi table documents
      struct document {
         uint64_t    id;
         uint64_t    offer_id;

         uint64_t primary_key() const { return id; }

         EOSLIB_SERIALIZE(document, (id)(offer_id))
      };

      typedef eosio::multi_index<N(orders), order> orders_table;
      typedef eosio::multi_index<N(offers), offer> offers_table;
      typedef eosio::multi_index<N(documents), document> documents_table;

      orders_table      orders;
      offers_table      offers;
      documents_table   documents;
};