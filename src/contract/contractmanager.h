#ifndef CONTRACTMANAGER_H
#define CONTRACTMANAGER_H

#include <memory>
#include <shared_mutex>
#include <unordered_map>

#include "abi.h"
#include "contract.h"

#include "../utils/db.h"
#include "../utils/options.h"
#include "../utils/safehash.h"
#include "../utils/strings.h"
#include "../utils/tx.h"
#include "../utils/utils.h"

// Forward declarations.
class rdPoS;
class State;
class ContractManagerInterface;

/**
 * Addresses for the contracts that are deployed at protocol level (contract name -> contract address).
 * That means these contracts are deployed at the beginning of the chain.
 * They cannot be destroyed nor dynamically deployed like other contracts.
 * Instead, they are deployed in the constructor of State.
 */
const std::unordered_map<std::string, Address> ProtocolContractAddresses = {
  {"rdPoS", Address(Hex::toBytes("0xb23aa52dbeda59277ab8a962c69f5971f22904cf"))}, // Sha3("randomDeterministicProofOfStake").substr(0,20)
  {"ContractManager", Address(Hex::toBytes("0x0001cb47ea6d8b55fe44fdd6b1bdb579efb43e61"))} // Sha3("ContractManager").substr(0,20)
};

/**
 * Class that holds all current contract instances in the blockchain state.
 * Responsible for being the factory and deploying contracts in the chain.
 * Also acts as an access point for contracts to access each other.
 */
class ContractManager : BaseContract {
  private:
    /**
     * Raw pointer to the blockchain state.
     * Used if the contract is a payable function.
     * Can be `nullptr` due to tests not requiring state (contract balance).
     */
    State* state;

    /**
     * Temporary map of balances within the chain.
     * Used during callContract with a payable function.
     * Cleared after the callContract function commited to the state if
     * `getCommit() == true` and the ethCall was successful.
     */
    std::unordered_map<Address, uint256_t, SafeHash> balances;

    /// Address of the contract that is currently being called.
    Address currentActiveContract = Address();

    /// List of currently deployed contracts.
    std::unordered_map<Address, std::unique_ptr<DynamicContract>, SafeHash> contracts;

    /// Mutex that manages read/write access to the contracts.
    mutable std::shared_mutex contractsMutex;

    /// Reference pointer to the rdPoS contract.
    const std::unique_ptr<rdPoS>& rdpos;

    /// Reference pointer to the options singleton.
    const std::unique_ptr<Options>& options;

    /// Derive a new contract address based on transaction sender and nonce.
    Address deriveContractAddress(const ethCallInfo &callInfo) const;

    /**
     * Create a new ERC20 contract. Solidity counterpart:
     * function createNewERC20Contract(string memory name, string memory symbol, uint8 decimals, uint256 supply) public {}
     */
    void createNewERC20Contract(const ethCallInfo &callInfo);

    /// Check if transaction can actually create a new ERC20 contract.
    void validateCreateNewERC20Contract(const ethCallInfo &callInfo) const;

    /**
     * Create a new ERC20Wrapper Contract. Solidity counterpart:
     * function createNewERC20WrapperContract() public {}
     */
    void createNewERC20WrapperContract(const ethCallInfo &callInfo);

    /// Check if transaction can actually create a new ERC20Wrapper contract.
    void validateCreateNewERC20WrapperContract(const ethCallInfo &callInfo) const;

    /**
     * Create a new ERC20NativeWrapper Contract. Solidity counterpart:
     * function createNewERC20NativeWrapperContract(string memory name, string memory symbol, uint8 decimals) public {}
     */
    void createNewERC20NativeWrapperContract(const ethCallInfo &callInfo);

    /// Check if transaction can actually create a new ERC20NativeWrapper contract.
    void validateCreateNewERC20NativeWrapperContract(const ethCallInfo &callInfo) const;

    /**
     * Get a serialized string with the deployed contracts. Solidity counterpart:
     * function getDeployedContracts() public view returns (string[] memory, address[] memory) {}
     */
    Bytes getDeployedContracts() const;

  private:
    std::unique_ptr<ContractManagerInterface> interface; ///< Interface to be passed to DynamicContract.

  public:
    /**
     * Constructor. Automatically loads contracts from the database and deploys them.
     * @param state Raw pointer to the state.
     * @param db Pointer to the database.
     * @param rdpos Pointer to the rdPoS contract.
     * @param options Pointer to the options singleton.
     */
    ContractManager(
      State* state, const std::unique_ptr<DB>& db,
      const std::unique_ptr<rdPoS>& rdpos, const std::unique_ptr<Options>& options
    );

    /// Destructor. Automatically saves contracts to the database before wiping them.
    ~ContractManager() override;

    /**
     * Override the default contract function call.
     * ContractManager processes things in a non-standard way (you cannot use
     * SafeVariables as contract creation actively writes to DB).
     * @param callInfo The call info to process.
     * @throw runtime_error if the call is not valid.
     */
    void ethCall(const ethCallInfo& callInfo) override;

    /**
     * Override the default contract view function call.
     * ContractManager process things in a non-standard way (you cannot use
     * SafeVariables as contract creation actively writes to DB).
     * @param data The call info to process.
     * @return A string with the requested info.
     * @throw std::runtime_error if the call is not valid.
     */
    const Bytes ethCallView(const ethCallInfo& data) const override;

    /**
     * Process a transaction that calls a function from a given contract.
     * @param tx The transaction to process.
     * @throw std::runtime_error if the call to the ethCall function fails.
     */
    void callContract(const TxBlock& tx);

    /**
     * Check if an ethCallInfo is trying to access a payable function.
     * @param callInfo The call info to check.
     * @return `true` if the function is payable, `false` otherwise.
     */
    bool isPayable(const ethCallInfo& callInfo) const;

    /**
     * Validate a transaction that calls a function from a given contract.
     * @param callInfo The call info to validate.
     * @return `true` if the transaction is valid, `false` otherwise.
     * @throw std::runtime_error if the validation fails.
     */
    bool validateCallContractWithTx(const ethCallInfo& callInfo);

    /**
     * Make an eth_call to a view function from the contract. Used by RPC.
     * @param callInfo The call info to process.
     * @return A string with the requested info.
     * @throw std::runtime_error if the call to the ethCall function fails
     * or if the contract does not exist.
     */
    const Bytes callContract(const ethCallInfo& callInfo) const;

    /**
     * Check if a transaction calls a contract
     * @param tx The transaction to check.
     * @return `true` if the transaction calls a contract, `false` otherwise.
     */
    bool isContractCall(const TxBlock& tx) const;

    /**
     * Check if an address is a contract.
     * @param address The address to check.
     * @return `true` if the address is a contract, `false` otherwise.
     */
    bool isContractAddress(const Address& address) const;

    /// Get a list of contract names and addresses.
    std::vector<std::pair<std::string, Address>> getContracts() const;

    ///< ContractManagerInterface is a friend so it can access private members.
    friend class ContractManagerInterface;
};

/// Interface class for DynamicContract to access ContractManager and interact with other dynamic contracts.
    class ContractManagerInterface {
      private:
        /// Reference to the contract manager.
        ContractManager& contractManager;

      public:
        /**
         * Constructor.
         * @param contractManager Reference to the contract manager.
         */
        explicit ContractManagerInterface(ContractManager& contractManager)
          : contractManager(contractManager)
        {}

        /// Populate a given address with its balance from the State.
        void populateBalance(const Address& address) const;

        /**
         * Call a contract function. Used by DynamicContract to call other contracts.
         * A given DynamicContract will only call another contract if
         * it was first triggered by a transaction.
         * That means we can use contractManager::commit() to know if
         * the call should commit or not.
         * This function will only be called if ContractManager::callContract()
         * or ContractManager::validateCallContractWithTx() was called before.
         * @param callInfo The call info.
         */
        void callContract(const ethCallInfo& callInfo);

        /**
         * Get a contract by its address.
         * Used by DynamicContract to access view/const functions of other contracts.
         * @tparam T The contract type.
         * @param address The address of the contract.
         * @return A pointer to the contract.
         * @throw runtime_error if contract is not found or not of the requested type.
         */
        template <typename T> const T* getContract(const Address &address) const {
          std::shared_lock<std::shared_mutex> lock(this->contractManager.contractsMutex);
          auto it = this->contractManager.contracts.find(address);
          if (it == this->contractManager.contracts.end()) throw std::runtime_error(
            "ContractManager::getContract: contract at address " +
            address.hex().get() + " not found."
          );
          T* ptr = dynamic_cast<T*>(it->second.get());
          if (ptr == nullptr) throw std::runtime_error(
            "ContractManager::getContract: Contract at address " +
            address.hex().get() + " is not of the requested type: " + typeid(T).name()
          );
          return ptr;
        }

        /**
         * Get the balance from a given address. Calls populateBalance(), so
         * it's technically the same as getting the balance directly from State.
         * Does NOT consider the current transaction being processed, if there is one.
         * @param address The address to get the balance from.
         * @return The balance of the address.
         */
        uint256_t getBalanceFromAddress(const Address& address) const;

        /**
         * Send tokens to a given address. Used by DynamicContract to send tokens to other contracts.
         * @param from The address from which the tokens will be sent from.
         * @param to The address to send the tokens to.
         * @param amount The amount of tokens to send.
         */
        void sendTokens(const Address& from, const Address& to, const uint256_t& amount);
    };

#endif // CONTRACTMANAGER_H
