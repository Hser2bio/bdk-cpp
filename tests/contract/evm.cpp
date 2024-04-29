/*
  Copyright (c) [2023-2024] [Sparq Network]
  This software is distributed under the MIT License.
  See the LICENSE.txt file in the project root for more information.
*/

#include "../../src/libs/catch2/catch_amalgamated.hpp"
#include "../../src/contract/templates/erc20.h"
#include "../../src/contract/abi.h"
#include "../../src/utils/db.h"
#include "../../src/utils/options.h"
#include "../../src/contract/templates/erc20wrapper.h"
#include "../../src/core/rdpos.h"
#include "../sdktestsuite.hpp"

namespace TERC721 {
  /*
   *
   * ERC20:
   * Constructor is called with argument "10000000000000000000000"
   *     // SPDX-  License-Identifier: MIT
   *     pragma solidity ^0.8.0;
   *
   *     import "@openzeppelin/contracts/token/ERC20/ERC20.sol";
   *
   *     contract ERC20Test is ERC20 {
   *         constructor(uint256 initialSupply) ERC20("TestToken", "TST") {
   *             _mint(msg.sender, initialSupply);
   *         }
   *     }
   */
  Bytes erc20bytecode = Hex::toBytes("0x608060405234801561000f575f80fd5b506040516115f23803806115f2833981810160405281019061003191906103aa565b6040518060400160405280600981526020017f54657374546f6b656e00000000000000000000000000000000000000000000008152506040518060400160405280600381526020017f545354000000000000000000000000000000000000000000000000000000000081525081600390816100ac9190610606565b5080600490816100bc9190610606565b5050506100cf33826100d560201b60201c565b506107ea565b5f73ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff1603610145575f6040517fec442f0500000000000000000000000000000000000000000000000000000000815260040161013c9190610714565b60405180910390fd5b6101565f838361015a60201b60201c565b5050565b5f73ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff16036101aa578060025f82825461019e919061075a565b92505081905550610278565b5f805f8573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f2054905081811015610233578381836040517fe450d38c00000000000000000000000000000000000000000000000000000000815260040161022a9392919061079c565b60405180910390fd5b8181035f808673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f2081905550505b5f73ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff16036102bf578060025f8282540392505081905550610309565b805f808473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f82825401925050819055505b8173ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff167fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef8360405161036691906107d1565b60405180910390a3505050565b5f80fd5b5f819050919050565b61038981610377565b8114610393575f80fd5b50565b5f815190506103a481610380565b92915050565b5f602082840312156103bf576103be610373565b5b5f6103cc84828501610396565b91505092915050565b5f81519050919050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52604160045260245ffd5b7f4e487b71000000000000000000000000000000000000000000000000000000005f52602260045260245ffd5b5f600282049050600182168061045057607f821691505b6020821081036104635761046261040c565b5b50919050565b5f819050815f5260205f209050919050565b5f6020601f8301049050919050565b5f82821b905092915050565b5f600883026104c57fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff8261048a565b6104cf868361048a565b95508019841693508086168417925050509392505050565b5f819050919050565b5f61050a61050561050084610377565b6104e7565b610377565b9050919050565b5f819050919050565b610523836104f0565b61053761052f82610511565b848454610496565b825550505050565b5f90565b61054b61053f565b61055681848461051a565b505050565b5b818110156105795761056e5f82610543565b60018101905061055c565b5050565b601f8211156105be5761058f81610469565b6105988461047b565b810160208510156105a7578190505b6105bb6105b38561047b565b83018261055b565b50505b505050565b5f82821c905092915050565b5f6105de5f19846008026105c3565b1980831691505092915050565b5f6105f683836105cf565b9150826002028217905092915050565b61060f826103d5565b67ffffffffffffffff811115610628576106276103df565b5b6106328254610439565b61063d82828561057d565b5f60209050601f83116001811461066e575f841561065c578287015190505b61066685826105eb565b8655506106cd565b601f19841661067c86610469565b5f5b828110156106a35784890151825560018201915060208501945060208101905061067e565b868310156106c057848901516106bc601f8916826105cf565b8355505b6001600288020188555050505b505050505050565b5f73ffffffffffffffffffffffffffffffffffffffff82169050919050565b5f6106fe826106d5565b9050919050565b61070e816106f4565b82525050565b5f6020820190506107275f830184610705565b92915050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52601160045260245ffd5b5f61076482610377565b915061076f83610377565b92508282019050808211156107875761078661072d565b5b92915050565b61079681610377565b82525050565b5f6060820190506107af5f830186610705565b6107bc602083018561078d565b6107c9604083018461078d565b949350505050565b5f6020820190506107e45f83018461078d565b92915050565b610dfb806107f75f395ff3fe608060405234801561000f575f80fd5b5060043610610091575f3560e01c8063313ce56711610064578063313ce5671461013157806370a082311461014f57806395d89b411461017f578063a9059cbb1461019d578063dd62ed3e146101cd57610091565b806306fdde0314610095578063095ea7b3146100b357806318160ddd146100e357806323b872dd14610101575b5f80fd5b61009d6101fd565b6040516100aa9190610a74565b60405180910390f35b6100cd60048036038101906100c89190610b25565b61028d565b6040516100da9190610b7d565b60405180910390f35b6100eb6102af565b6040516100f89190610ba5565b60405180910390f35b61011b60048036038101906101169190610bbe565b6102b8565b6040516101289190610b7d565b60405180910390f35b6101396102e6565b6040516101469190610c29565b60405180910390f35b61016960048036038101906101649190610c42565b6102ee565b6040516101769190610ba5565b60405180910390f35b610187610333565b6040516101949190610a74565b60405180910390f35b6101b760048036038101906101b29190610b25565b6103c3565b6040516101c49190610b7d565b60405180910390f35b6101e760048036038101906101e29190610c6d565b6103e5565b6040516101f49190610ba5565b60405180910390f35b60606003805461020c90610cd8565b80601f016020809104026020016040519081016040528092919081815260200182805461023890610cd8565b80156102835780601f1061025a57610100808354040283529160200191610283565b820191905f5260205f20905b81548152906001019060200180831161026657829003601f168201915b5050505050905090565b5f80610297610467565b90506102a481858561046e565b600191505092915050565b5f600254905090565b5f806102c2610467565b90506102cf858285610480565b6102da858585610512565b60019150509392505050565b5f6012905090565b5f805f8373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f20549050919050565b60606004805461034290610cd8565b80601f016020809104026020016040519081016040528092919081815260200182805461036e90610cd8565b80156103b95780601f10610390576101008083540402835291602001916103b9565b820191905f5260205f20905b81548152906001019060200180831161039c57829003601f168201915b5050505050905090565b5f806103cd610467565b90506103da818585610512565b600191505092915050565b5f60015f8473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f8373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f2054905092915050565b5f33905090565b61047b8383836001610602565b505050565b5f61048b84846103e5565b90507fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff811461050c57818110156104fd578281836040517ffb8f41b20000000000000000000000000000000000000000000000000000000081526004016104f493929190610d17565b60405180910390fd5b61050b84848484035f610602565b5b50505050565b5f73ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1603610582575f6040517f96c6fd1e0000000000000000000000000000000000000000000000000000000081526004016105799190610d4c565b60405180910390fd5b5f73ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff16036105f2575f6040517fec442f050000000000000000000000000000000000000000000000000000000081526004016105e99190610d4c565b60405180910390fd5b6105fd8383836107d1565b505050565b5f73ffffffffffffffffffffffffffffffffffffffff168473ffffffffffffffffffffffffffffffffffffffff1603610672575f6040517fe602df050000000000000000000000000000000000000000000000000000000081526004016106699190610d4c565b60405180910390fd5b5f73ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff16036106e2575f6040517f94280d620000000000000000000000000000000000000000000000000000000081526004016106d99190610d4c565b60405180910390fd5b8160015f8673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f8573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f208190555080156107cb578273ffffffffffffffffffffffffffffffffffffffff168473ffffffffffffffffffffffffffffffffffffffff167f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925846040516107c29190610ba5565b60405180910390a35b50505050565b5f73ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1603610821578060025f8282546108159190610d92565b925050819055506108ef565b5f805f8573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f20549050818110156108aa578381836040517fe450d38c0000000000000000000000000000000000000000000000000000000081526004016108a193929190610d17565b60405180910390fd5b8181035f808673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f2081905550505b5f73ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff1603610936578060025f8282540392505081905550610980565b805f808473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020015f205f82825401925050819055505b8173ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff167fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef836040516109dd9190610ba5565b60405180910390a3505050565b5f81519050919050565b5f82825260208201905092915050565b5f5b83811015610a21578082015181840152602081019050610a06565b5f8484015250505050565b5f601f19601f8301169050919050565b5f610a46826109ea565b610a5081856109f4565b9350610a60818560208601610a04565b610a6981610a2c565b840191505092915050565b5f6020820190508181035f830152610a8c8184610a3c565b905092915050565b5f80fd5b5f73ffffffffffffffffffffffffffffffffffffffff82169050919050565b5f610ac182610a98565b9050919050565b610ad181610ab7565b8114610adb575f80fd5b50565b5f81359050610aec81610ac8565b92915050565b5f819050919050565b610b0481610af2565b8114610b0e575f80fd5b50565b5f81359050610b1f81610afb565b92915050565b5f8060408385031215610b3b57610b3a610a94565b5b5f610b4885828601610ade565b9250506020610b5985828601610b11565b9150509250929050565b5f8115159050919050565b610b7781610b63565b82525050565b5f602082019050610b905f830184610b6e565b92915050565b610b9f81610af2565b82525050565b5f602082019050610bb85f830184610b96565b92915050565b5f805f60608486031215610bd557610bd4610a94565b5b5f610be286828701610ade565b9350506020610bf386828701610ade565b9250506040610c0486828701610b11565b9150509250925092565b5f60ff82169050919050565b610c2381610c0e565b82525050565b5f602082019050610c3c5f830184610c1a565b92915050565b5f60208284031215610c5757610c56610a94565b5b5f610c6484828501610ade565b91505092915050565b5f8060408385031215610c8357610c82610a94565b5b5f610c9085828601610ade565b9250506020610ca185828601610ade565b9150509250929050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52602260045260245ffd5b5f6002820490506001821680610cef57607f821691505b602082108103610d0257610d01610cab565b5b50919050565b610d1181610ab7565b82525050565b5f606082019050610d2a5f830186610d08565b610d376020830185610b96565b610d446040830184610b96565b949350505050565b5f602082019050610d5f5f830184610d08565b92915050565b7f4e487b71000000000000000000000000000000000000000000000000000000005f52601160045260245ffd5b5f610d9c82610af2565b9150610da783610af2565b9250828201905080821115610dbf57610dbe610d65565b5b9291505056fea264697066735822122043402e069181d2f0057dcffd90d442615a3da729849963e98eada0e05475373164736f6c6343000819003300000000000000000000000000000000000000000000021e19e0c9bab2400000");
  /**
   * ERC20Wrapper:
   *     pragma solidity ^0.8.0;
   *
   *     import "@openzeppelin/contracts/token/ERC20/IERC20.sol";
   *
   *     contract ERC20Wrapper {
   *       // Nested mapping to hold user balances for each token
   *       mapping(address => mapping(address => uint256)) private _balances;
   *
   *       // Event declarations for Deposits and Withdrawals
   *       event Deposit(address indexed user, address indexed token, uint256 amount);
   *       event Withdrawal(address indexed user, address indexed token, uint256 amount);
   *
   *       // Function to deposit ERC20 tokens into this contract
   *       function deposit(address token, uint256 amount) public {
   *         require(amount > 0, "Amount must be greater than zero");
   *         require(IERC20(token).transferFrom(msg.sender, address(this), amount), "Transfer failed");
   *
   *         _balances[msg.sender][token] += amount;
   *         emit Deposit(msg.sender, token, amount);
   *       }
   *
   *       // Function to withdraw ERC20 tokens from this contract
   *       function withdraw(address token, uint256 amount) public {
   *         require(amount > 0, "Amount must be greater than zero");
   *         require(_balances[msg.sender][token] >= amount, "Insufficient balance");
   *
   *         _balances[msg.sender][token] -= amount;
   *         require(IERC20(token).transfer(msg.sender, amount), "Transfer failed");
   *
   *         emit Withdrawal(msg.sender, token, amount);
   *       }
   *
   *       // Function to check the balance of a user for a specific token
   *       function balanceOf(address user, address token) public view returns (uint256) {
   *         return _balances[user][token];
   *       }
   *
   *       // Function to check the balance of this contract for a specific token
   *       function contractBalance(address token) public view returns (uint256) {
   *         return IERC20(token).balanceOf(address(this));
   *       }
   *     }
   */

  // Due to C++ template mechanics, we can feed C++ functions to templates
  // But these functions needs to have the same Solidity declaration
  // Basically, an interface, but with minimal definition so the compiler doesnt go undefined
  class SolERC20Wrapper {
    public:
      void deposit(const Address& token, const uint256_t& amount) {};
      void withdraw(const Address& token, const uint256_t& amount) {};
      uint256_t balanceOf(const Address& user, const Address& token) const { return 0; };
      uint256_t contractBalance(const Address& token) const { return 0; };
      void static registerContract() {
        ContractReflectionInterface::registerContractMethods<
          SolERC20Wrapper
        >(
          std::vector<std::string>{},
          std::make_tuple("deposit", &SolERC20Wrapper::deposit, FunctionTypes::View, std::vector<std::string>{"token", "amount"}),
          std::make_tuple("withdraw", &SolERC20Wrapper::withdraw, FunctionTypes::View, std::vector<std::string>{"token", "amount"}),
          std::make_tuple("balanceOf", &SolERC20Wrapper::balanceOf, FunctionTypes::View, std::vector<std::string>{"user","token"}),
          std::make_tuple("contractBalance", &SolERC20Wrapper::contractBalance, FunctionTypes::View, std::vector<std::string>{"token"})
        );
      }
  };

  Bytes erc20WrapperBytes = Hex::toBytes("0x6080604052348015600e575f80fd5b506105cd8061001c5f395ff3fe608060405234801561000f575f80fd5b506004361061004a575f3560e01c806347e7ef241461004e578063c53b770214610063578063f3fef3a314610088578063f7888aec1461009b575b5f80fd5b61006161005c3660046104ae565b6100d1565b005b6100766100713660046104d6565b610251565b60405190815260200160405180910390f35b6100616100963660046104ae565b6102bf565b6100766100a93660046104f6565b6001600160a01b039182165f9081526020818152604080832093909416825291909152205490565b5f81116101255760405162461bcd60e51b815260206004820181905260248201527f416d6f756e74206d7573742062652067726561746572207468616e207a65726f60448201526064015b60405180910390fd5b6040516323b872dd60e01b8152336004820152306024820152604481018290526001600160a01b038316906323b872dd906064016020604051808303815f875af1158015610175573d5f803e3d5ffd5b505050506040513d601f19601f820116820180604052508101906101999190610527565b6101d75760405162461bcd60e51b815260206004820152600f60248201526e151c985b9cd9995c8819985a5b1959608a1b604482015260640161011c565b335f908152602081815260408083206001600160a01b03861684529091528120805483929061020790849061055a565b90915550506040518181526001600160a01b0383169033907f5548c837ab068cf56a2c2479df0882a4922fd203edb7517321831d95078c5f62906020015b60405180910390a35050565b6040516370a0823160e01b81523060048201525f906001600160a01b038316906370a0823190602401602060405180830381865afa158015610295573d5f803e3d5ffd5b505050506040513d601f19601f820116820180604052508101906102b9919061056d565b92915050565b5f811161030e5760405162461bcd60e51b815260206004820181905260248201527f416d6f756e74206d7573742062652067726561746572207468616e207a65726f604482015260640161011c565b335f908152602081815260408083206001600160a01b03861684529091529020548111156103755760405162461bcd60e51b8152602060048201526014602482015273496e73756666696369656e742062616c616e636560601b604482015260640161011c565b335f908152602081815260408083206001600160a01b0386168452909152812080548392906103a5908490610584565b909155505060405163a9059cbb60e01b8152336004820152602481018290526001600160a01b0383169063a9059cbb906044016020604051808303815f875af11580156103f4573d5f803e3d5ffd5b505050506040513d601f19601f820116820180604052508101906104189190610527565b6104565760405162461bcd60e51b815260206004820152600f60248201526e151c985b9cd9995c8819985a5b1959608a1b604482015260640161011c565b6040518181526001600160a01b0383169033907f2717ead6b9200dd235aad468c9809ea400fe33ac69b5bfaa6d3e90fc922b639890602001610245565b80356001600160a01b03811681146104a9575f80fd5b919050565b5f80604083850312156104bf575f80fd5b6104c883610493565b946020939093013593505050565b5f602082840312156104e6575f80fd5b6104ef82610493565b9392505050565b5f8060408385031215610507575f80fd5b61051083610493565b915061051e60208401610493565b90509250929050565b5f60208284031215610537575f80fd5b815180151581146104ef575f80fd5b634e487b7160e01b5f52601160045260245ffd5b808201808211156102b9576102b9610546565b5f6020828403121561057d575f80fd5b5051919050565b818103818111156102b9576102b961054656fea2646970667358221220a682d87f949af3271670e7b8a26e66a974e4f99da4daccd6eeee9f51c980d6bb64736f6c63430008190033");
  TEST_CASE("EVM ERC20 Tests", "[contract][evm]") {
    SECTION("ERC20 Creation + load from DB.") {
      std::unique_ptr<Options> options = nullptr;
      Address erc20Address = Address();
      {
        SDKTestSuite sdk = SDKTestSuite::createNewEnvironment("testERC20CreationEVM");
        // const TestAccount& from, const Address& to, const uint256_t& value, Bytes data = Bytes()
        erc20Address = sdk.deployBytecode(erc20bytecode);
        // Now for the funny part, we are NOT a C++ contract, but we can
        // definitely take advantage of the templated ABI to interact with it
        // as the encoding is the same

        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::name) == "TestToken");
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::symbol) == "TST");
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::decimals) == 18);
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::totalSupply) == uint256_t("10000000000000000000000"));
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("10000000000000000000000"));

        // Make a copy of the current options
        options = std::make_unique<Options>(sdk.getOptions());
        // Dump the state
        sdk.getState().saveToDB();
      }

      /// SDKTestSuite should automatically load the state from the DB if we
      /// Construct it with a options object
      /// (The createNewEnvironment DELETES the DB if any is found)
      SDKTestSuite sdk(*options);

      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::name) == "TestToken");
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::symbol) == "TST");
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::decimals) == 18);
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::totalSupply) == uint256_t("10000000000000000000000"));
    }

    SECTION("ERC20 transfer + load from DB.") {
      std::unique_ptr<Options> options = nullptr;
      Address erc20Address = Address();
      Address to(Utils::randBytes(20));
      {
        SDKTestSuite sdk = SDKTestSuite::createNewEnvironment("testERC20TransferEVM");
        // const TestAccount& from, const Address& to, const uint256_t& value, Bytes data = Bytes()
        erc20Address = sdk.deployBytecode(erc20bytecode);
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::name) == "TestToken");
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::symbol) == "TST");
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::decimals) == 18);
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::totalSupply) == uint256_t("10000000000000000000000"));
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("10000000000000000000000"));
        Address owner = sdk.getChainOwnerAccount().address;

        uint256_t balanceMe = sdk.callViewFunction(erc20Address, &ERC20::balanceOf, owner);
        uint256_t balanceTo = sdk.callViewFunction(erc20Address, &ERC20::balanceOf, to);
        REQUIRE(balanceMe == uint256_t("10000000000000000000000")); // 10000 TST
        REQUIRE(balanceTo == 0);

        Hash transferTx = sdk.callFunction(erc20Address, &ERC20::transfer, to, uint256_t("5000000000000000000000")); // 5000 TST
        balanceMe = sdk.callViewFunction(erc20Address, &ERC20::balanceOf, owner);
        balanceTo = sdk.callViewFunction(erc20Address, &ERC20::balanceOf, to);
        REQUIRE(balanceMe == uint256_t("5000000000000000000000"));
        REQUIRE(balanceTo == uint256_t("5000000000000000000000"));

        // "owner" doesn't have enough balance, this should throw and balances should stay intact
        REQUIRE_THROWS(sdk.callFunction(erc20Address, &ERC20::transfer, to, uint256_t("5000000000000000000001")));

        balanceMe = sdk.callViewFunction(erc20Address, &ERC20::balanceOf, owner);
        balanceTo = sdk.callViewFunction(erc20Address, &ERC20::balanceOf, to);
        REQUIRE(balanceMe == uint256_t("5000000000000000000000"));
        REQUIRE(balanceTo == uint256_t("5000000000000000000000"));
        // Copy the SDK options
        options = std::make_unique<Options>(sdk.getOptions());
        sdk.getState().saveToDB();
      }

      SDKTestSuite sdk(*options);

      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::name) == "TestToken");
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::symbol) == "TST");
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::decimals) == 18);
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::totalSupply) == uint256_t("10000000000000000000000"));
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("5000000000000000000000"));
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, to) == uint256_t("5000000000000000000000"));
    }

    SECTION("EVM -> CPP Calls (EVM ERC20Wrapper) + load from DB.") {
      std::unique_ptr<Options> options = nullptr;
      Address erc20Address = Address();
      Address erc20WrapperAddress = Address();
      {
        SDKTestSuite sdk = SDKTestSuite::createNewEnvironment("testEVMtoCPPcalls");
        erc20Address = sdk.deployContract<ERC20>(std::string("TestToken"), std::string("TST"), uint8_t(18), uint256_t("10000000000000000000000"));
        erc20WrapperAddress = sdk.deployBytecode(erc20WrapperBytes);

        sdk.callFunction(erc20Address, &ERC20::approve, erc20WrapperAddress, uint256_t("10000000000000000000000"));
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::allowance, sdk.getChainOwnerAccount().address, erc20WrapperAddress) == uint256_t("10000000000000000000000"));

        REQUIRE(sdk.callViewFunction(erc20WrapperAddress, &SolERC20Wrapper::balanceOf, sdk.getChainOwnerAccount().address, erc20Address) == uint256_t("0"));
        REQUIRE(sdk.callViewFunction(erc20WrapperAddress, &SolERC20Wrapper::contractBalance, erc20Address) == uint256_t("0"));
        REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("10000000000000000000000"));
        REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, erc20WrapperAddress) == uint256_t("0"));

        auto deposit = sdk.callFunction(erc20WrapperAddress, &SolERC20Wrapper::deposit, erc20Address, uint256_t("10000000000000000000000"));
        REQUIRE(sdk.callViewFunction(erc20WrapperAddress, &SolERC20Wrapper::balanceOf, sdk.getChainOwnerAccount().address, erc20Address) == uint256_t("10000000000000000000000"));
        REQUIRE(sdk.callViewFunction(erc20WrapperAddress, &SolERC20Wrapper::contractBalance, erc20Address) == uint256_t("10000000000000000000000"));
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("0"));
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::balanceOf, erc20WrapperAddress) == uint256_t("10000000000000000000000"));

        // Withdraw the 1/3 of what we have deposited
        auto withdraw = sdk.callFunction(erc20WrapperAddress, &SolERC20Wrapper::withdraw, erc20Address, uint256_t("3333333333333333333333"));
        REQUIRE(sdk.callViewFunction(erc20WrapperAddress, &SolERC20Wrapper::balanceOf, sdk.getChainOwnerAccount().address, erc20Address) == uint256_t("6666666666666666666667"));
        REQUIRE(sdk.callViewFunction(erc20WrapperAddress, &SolERC20Wrapper::contractBalance, erc20Address) == uint256_t("6666666666666666666667"));
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("3333333333333333333333"));
        REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::balanceOf, erc20WrapperAddress) == uint256_t("6666666666666666666667"));

        options = std::make_unique<Options>(sdk.getOptions());
        sdk.getState().saveToDB();
      }

      SDKTestSuite sdk(*options);

      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::name) == "TestToken");
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::symbol) == "TST");
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::decimals) == 18);
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::totalSupply) == uint256_t("10000000000000000000000"));

      REQUIRE (sdk.callViewFunction(erc20WrapperAddress, &SolERC20Wrapper::balanceOf, sdk.getChainOwnerAccount().address, erc20Address) == uint256_t("6666666666666666666667"));
      REQUIRE (sdk.callViewFunction(erc20WrapperAddress, &SolERC20Wrapper::contractBalance, erc20Address) == uint256_t("6666666666666666666667"));
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("3333333333333333333333"));
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, erc20WrapperAddress) == uint256_t("6666666666666666666667"));

    }

    SECTION("CPP -> EVM Calls (CPP ERC20Wrapper) + load from DB.") {
      std::unique_ptr<Options> options = nullptr;
      Address erc20Address = Address();
      Address erc20WrapperAddress = Address();
      {
        auto sdk = SDKTestSuite::createNewEnvironment("testCPPtoEVMcalls");
        erc20Address = sdk.deployBytecode(erc20bytecode);
        erc20WrapperAddress = sdk.deployContract<ERC20Wrapper>();

        sdk.callFunction(erc20Address, &ERC20::approve, erc20WrapperAddress, uint256_t("10000000000000000000000"));
        REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::allowance, sdk.getChainOwnerAccount().address, erc20WrapperAddress) == uint256_t("10000000000000000000000"));

        REQUIRE (sdk.callViewFunction(erc20WrapperAddress, &ERC20Wrapper::getUserBalance, erc20Address, sdk.getChainOwnerAccount().address) == uint256_t("0"));
        REQUIRE (sdk.callViewFunction(erc20WrapperAddress, &ERC20Wrapper::getContractBalance, erc20Address) == uint256_t("0"));
        REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("10000000000000000000000"));
        REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, erc20WrapperAddress) == uint256_t("0"));

        auto deposit = sdk.callFunction(erc20WrapperAddress, &ERC20Wrapper::deposit, erc20Address, uint256_t("10000000000000000000000"));
        REQUIRE (sdk.callViewFunction(erc20WrapperAddress, &ERC20Wrapper::getUserBalance, erc20Address, sdk.getChainOwnerAccount().address) == uint256_t("10000000000000000000000"));
        REQUIRE (sdk.callViewFunction(erc20WrapperAddress, &ERC20Wrapper::getContractBalance, erc20Address) == uint256_t("10000000000000000000000"));
        REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("0"));
        REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, erc20WrapperAddress) == uint256_t("10000000000000000000000"));

        // Withdraw the 1/3 of what we have deposited
        auto withdraw = sdk.callFunction(erc20WrapperAddress, &ERC20Wrapper::withdraw, erc20Address, uint256_t("3333333333333333333333"));
        REQUIRE (sdk.callViewFunction(erc20WrapperAddress, &ERC20Wrapper::getUserBalance, erc20Address, sdk.getChainOwnerAccount().address) == uint256_t("6666666666666666666667"));
        REQUIRE (sdk.callViewFunction(erc20WrapperAddress, &ERC20Wrapper::getContractBalance, erc20Address) == uint256_t("6666666666666666666667"));
        REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("3333333333333333333333"));
        REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, erc20WrapperAddress) == uint256_t("6666666666666666666667"));

        options = std::make_unique<Options>(sdk.getOptions());
        sdk.getState().saveToDB();
      }

      SDKTestSuite sdk(*options);

      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::name) == "TestToken");
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::symbol) == "TST");
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::decimals) == 18);
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::totalSupply) == uint256_t("10000000000000000000000"));

      REQUIRE (sdk.callViewFunction(erc20WrapperAddress, &ERC20Wrapper::getUserBalance, erc20Address, sdk.getChainOwnerAccount().address) == uint256_t("6666666666666666666667"));
      REQUIRE (sdk.callViewFunction(erc20WrapperAddress, &ERC20Wrapper::getContractBalance, erc20Address) == uint256_t("6666666666666666666667"));
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("3333333333333333333333"));
      REQUIRE (sdk.callViewFunction(erc20Address, &ERC20::balanceOf, erc20WrapperAddress) == uint256_t("6666666666666666666667"));
    }
  }
}