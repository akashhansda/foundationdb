/*
 * SimpleConfigTransaction.h
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2018 Apple Inc. and the FoundationDB project authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <memory>

#include "fdbclient/CommitTransaction.h"
#include "fdbclient/ConfigTransactionInterface.h"
#include "fdbclient/CoordinationInterface.h"
#include "fdbclient/FDBTypes.h"
#include "fdbclient/ISingleThreadTransaction.h"
#include "flow/Error.h"
#include "flow/flow.h"

class SimpleConfigTransaction final : public ISingleThreadTransaction, public FastAllocated<SimpleConfigTransaction> {
	std::unique_ptr<class SimpleConfigTransactionImpl> impl;

public:
	SimpleConfigTransaction(ClusterConnectionString const&);
	~SimpleConfigTransaction();
	void setVersion(Version) override { throw client_invalid_operation(); }
	Future<Version> getReadVersion() override;
	Optional<Version> getCachedReadVersion() override;

	Future<Optional<Value>> get(Key const& key, bool snapshot = false) override;
	Future<Key> getKey(KeySelector const& key, bool snapshot = false) override { throw client_invalid_operation(); }
	Future<Standalone<RangeResultRef>> getRange(KeySelector const& begin,
	                                            KeySelector const& end,
	                                            int limit,
	                                            bool snapshot = false,
	                                            bool reverse = false) override;
	Future<Standalone<RangeResultRef>> getRange(KeySelector begin,
	                                            KeySelector end,
	                                            GetRangeLimits limits,
	                                            bool snapshot = false,
	                                            bool reverse = false) override;
	Future<Standalone<VectorRef<const char*>>> getAddressesForKey(Key const& key) override {
		throw client_invalid_operation();
	}
	Future<Standalone<VectorRef<KeyRef>>> getRangeSplitPoints(KeyRange const& range, int64_t chunkSize) override {
		throw client_invalid_operation();
	}
	Future<int64_t> getEstimatedRangeSizeBytes(KeyRange const& keys) override { throw client_invalid_operation(); }
	void addReadConflictRange(KeyRangeRef const& keys) override { throw client_invalid_operation(); }
	void makeSelfConflicting() override { throw client_invalid_operation(); }
	void atomicOp(KeyRef const& key, ValueRef const& operand, uint32_t operationType) override {
		throw client_invalid_operation();
	}
	void set(KeyRef const& key, ValueRef const& value) override;
	void clear(KeyRangeRef const& range) override { throw client_invalid_operation(); }
	void clear(KeyRef const& key) override;
	Future<Void> watch(Key const& key) override { throw client_invalid_operation(); }
	void addWriteConflictRange(KeyRangeRef const& keys) override { throw client_invalid_operation(); }
	Future<Void> commit() override;
	Version getCommittedVersion() const override;
	int64_t getApproximateSize() const override;
	Future<Standalone<StringRef>> getVersionstamp() override { throw client_invalid_operation(); }
	void setOption(FDBTransactionOptions::Option option, Optional<StringRef> value = Optional<StringRef>()) override;
	Future<Void> onError(Error const& e) override;
	void cancel() override;
	void reset() override;
	void debugTransaction(UID dID) override;
	void checkDeferredError() override;
	void getWriteConflicts(KeyRangeMap<bool>* result) override;
	void fullReset();
};
