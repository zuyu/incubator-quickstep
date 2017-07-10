/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_QUERY_EXECUTION_QUERY_MANAGER_SINGLE_NODE_HPP_
#define QUICKSTEP_QUERY_EXECUTION_QUERY_MANAGER_SINGLE_NODE_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionState.hpp"
#include "query_execution/QueryManagerBase.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogDatabase;
class CatalogDatabaseLite;
class QueryHandle;
class StorageManager;
class WorkerMessage;

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief A class that manages the execution of a query including generation
 *        of new work orders, keeping track of the query exection state.
 **/
class QueryManagerSingleNode final : public QueryManagerBase {
 public:
  /**
   * @brief Constructor.
   *
   * @param foreman_client_id The TMB client ID of the foreman thread.
   * @param num_numa_nodes The number of NUMA nodes used by the system.
   * @param query_handle The QueryHandle object for this query.
   * @param catalog_database The CatalogDatabse used by the query.
   * @param storage_manager The StorageManager used by the query.
   * @param bus The TMB used for communication.
   **/
  QueryManagerSingleNode(const tmb::client_id foreman_client_id,
                         const std::size_t num_numa_nodes,
                         QueryHandle *query_handle,
                         CatalogDatabaseLite *catalog_database,
                         StorageManager *storage_manager,
                         tmb::MessageBus *bus);

  ~QueryManagerSingleNode() override {}

  bool fetchNormalWorkOrders(const dag_node_index index,
                             const partition_id part_id) override;

 /**
   * @brief Get the next workorder to be excuted, wrapped in a WorkerMessage.
   *
   * @param start_operator_index Begin the search for the schedulable WorkOrder
   *        with the operator at this index.
   * @param numa_node The next WorkOrder should preferably have its input(s)
   *        from this numa_node. This is a hint and not a binding requirement.
   *
   * @return A pointer to the WorkerMessage. If there's no WorkOrder to be
   *         executed, return NULL.
   **/
  WorkerMessage* getNextWorkerMessage(
      const dag_node_index start_operator_index);

  /**
   * @brief Get a pointer to the QueryContext.
   **/
  inline QueryContext* getQueryContextMutable() {
    return query_context_.get();
  }

  std::size_t getQueryMemoryConsumptionBytes() const override;

 private:
  bool checkNormalExecutionOver(const dag_node_index index, const partition_id part_id) const override {
    return (checkAllDependenciesMet(index, part_id) &&
            !workorders_container_->hasNormalWorkOrder(index, part_id) &&
            query_exec_state_->getNumQueuedWorkOrders(index, part_id) == 0 &&
            query_exec_state_->hasDoneGenerationWorkOrders(index, part_id));
  }

  bool initiateRebuild(const dag_node_index index,
                       const partition_id part_id) override;

  bool initiateRebuild(const dag_node_index index) override;

  bool checkRebuildOver(const dag_node_index index,
                        const partition_id part_id) const override {
    return query_exec_state_->hasRebuildInitiated(index, part_id) &&
           !workorders_container_->hasRebuildWorkOrder(index, part_id) &&
           (query_exec_state_->getNumRebuildWorkOrders(index, part_id) == 0);
  }

  bool checkRebuildOver(const dag_node_index index) const override {
    const auto cit = output_num_partitions_.find(index);
    DCHECK(cit != output_num_partitions_.end());
    for (partition_id output_part_id = 0; output_part_id < cit->second; ++output_part_id) {
      if (!checkRebuildOver(index, output_part_id)) {
        return false;
      }
    }

    return true;
  }

  /**
   * @brief Get the rebuild WorkOrders for an operator.
   *
   * @note This function should be called only once, when all the normal
   *       WorkOrders generated by an operator finish their execution.
   *
   * @param index The index of the operator in the query plan DAG.
   * @param container A pointer to a WorkOrdersContainer to be used to store the
   *        generated WorkOrders.
   **/
  void getRebuildWorkOrders(const dag_node_index index,
                            const partition_id part_id,
                            WorkOrdersContainer *container);

  /**
   * @brief Get the total memory (in bytes) occupied by temporary relations
   *        created during the query execution.
   **/
  std::size_t getTotalTempRelationMemoryInBytes() const;

  const tmb::client_id foreman_client_id_;

  StorageManager *storage_manager_;
  tmb::MessageBus *bus_;

  std::unique_ptr<QueryContext> query_context_;

  std::unique_ptr<WorkOrdersContainer> workorders_container_;

  const CatalogDatabase &database_;

  DISALLOW_COPY_AND_ASSIGN(QueryManagerSingleNode);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_QUERY_MANAGER_SINGLE_NODE_HPP_
