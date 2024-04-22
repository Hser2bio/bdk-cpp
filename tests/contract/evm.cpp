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
#include "../../src/contract/contractmanager.h"
#include "../../src/core/rdpos.h"
#include "../sdktestsuite.hpp"


/*
 * Solidity Source Code:
 * // SPDX-License-Identifier: MIT
 * pragma solidity ^0.8.0;
 *
 * import "@openzeppelin/contracts/token/ERC20/ERC20.sol";
 *
 * contract ERC20Test is ERC20 {
 *     constructor(uint256 initialSupply) ERC20("TestToken", "TST") {
 *         _mint(msg.sender, initialSupply);
 *     }
 * }
 *
 * Constructor is called with argument "10000000000000000000000"
 */
namespace TERC721 {
  Bytes erc20bytecode = Hex::toBytes("0x608060405234801561000f575f80fd5b50604051610a9d380380610a9d83398101604081905261002e91610204565b604051806040016040528060098152602001682a32b9ba2a37b5b2b760b91b815250604051806040016040528060038152602001621514d560ea1b815250816003908161007b91906102b3565b50600461008882826102b3565b50505061009b33826100a160201b60201c565b50610397565b6001600160a01b0382166100cf5760405163ec442f0560e01b81525f60048201526024015b60405180910390fd5b6100da5f83836100de565b5050565b6001600160a01b038316610108578060025f8282546100fd9190610372565b909155506101789050565b6001600160a01b0383165f908152602081905260409020548181101561015a5760405163391434e360e21b81526001600160a01b038516600482015260248101829052604481018390526064016100c6565b6001600160a01b0384165f9081526020819052604090209082900390555b6001600160a01b038216610194576002805482900390556101b2565b6001600160a01b0382165f9081526020819052604090208054820190555b816001600160a01b0316836001600160a01b03167fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef836040516101f791815260200190565b60405180910390a3505050565b5f60208284031215610214575f80fd5b5051919050565b634e487b7160e01b5f52604160045260245ffd5b600181811c9082168061024357607f821691505b60208210810361026157634e487b7160e01b5f52602260045260245ffd5b50919050565b601f8211156102ae57805f5260205f20601f840160051c8101602085101561028c5750805b601f840160051c820191505b818110156102ab575f8155600101610298565b50505b505050565b81516001600160401b038111156102cc576102cc61021b565b6102e0816102da845461022f565b84610267565b602080601f831160018114610313575f84156102fc5750858301515b5f19600386901b1c1916600185901b17855561036a565b5f85815260208120601f198616915b8281101561034157888601518255948401946001909101908401610322565b508582101561035e57878501515f19600388901b60f8161c191681555b505060018460011b0185555b505050505050565b8082018082111561039157634e487b7160e01b5f52601160045260245ffd5b92915050565b6106f9806103a45f395ff3fe608060405234801561000f575f80fd5b5060043610610090575f3560e01c8063313ce56711610063578063313ce567146100fa57806370a082311461010957806395d89b4114610131578063a9059cbb14610139578063dd62ed3e1461014c575f80fd5b806306fdde0314610094578063095ea7b3146100b257806318160ddd146100d557806323b872dd146100e7575b5f80fd5b61009c610184565b6040516100a99190610553565b60405180910390f35b6100c56100c03660046105ba565b610214565b60405190151581526020016100a9565b6002545b6040519081526020016100a9565b6100c56100f53660046105e2565b61022d565b604051601281526020016100a9565b6100d961011736600461061b565b6001600160a01b03165f9081526020819052604090205490565b61009c610250565b6100c56101473660046105ba565b61025f565b6100d961015a36600461063b565b6001600160a01b039182165f90815260016020908152604080832093909416825291909152205490565b6060600380546101939061066c565b80601f01602080910402602001604051908101604052809291908181526020018280546101bf9061066c565b801561020a5780601f106101e15761010080835404028352916020019161020a565b820191905f5260205f20905b8154815290600101906020018083116101ed57829003601f168201915b5050505050905090565b5f3361022181858561026c565b60019150505b92915050565b5f3361023a85828561027e565b6102458585856102fe565b506001949350505050565b6060600480546101939061066c565b5f336102218185856102fe565b610279838383600161035b565b505050565b6001600160a01b038381165f908152600160209081526040808320938616835292905220545f1981146102f857818110156102ea57604051637dc7a0d960e11b81526001600160a01b038416600482015260248101829052604481018390526064015b60405180910390fd5b6102f884848484035f61035b565b50505050565b6001600160a01b03831661032757604051634b637e8f60e11b81525f60048201526024016102e1565b6001600160a01b0382166103505760405163ec442f0560e01b81525f60048201526024016102e1565b61027983838361042d565b6001600160a01b0384166103845760405163e602df0560e01b81525f60048201526024016102e1565b6001600160a01b0383166103ad57604051634a1406b160e11b81525f60048201526024016102e1565b6001600160a01b038085165f90815260016020908152604080832093871683529290522082905580156102f857826001600160a01b0316846001600160a01b03167f8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b9258460405161041f91815260200190565b60405180910390a350505050565b6001600160a01b038316610457578060025f82825461044c91906106a4565b909155506104c79050565b6001600160a01b0383165f90815260208190526040902054818110156104a95760405163391434e360e21b81526001600160a01b038516600482015260248101829052604481018390526064016102e1565b6001600160a01b0384165f9081526020819052604090209082900390555b6001600160a01b0382166104e357600280548290039055610501565b6001600160a01b0382165f9081526020819052604090208054820190555b816001600160a01b0316836001600160a01b03167fddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef8360405161054691815260200190565b60405180910390a3505050565b5f602080835283518060208501525f5b8181101561057f57858101830151858201604001528201610563565b505f604082860101526040601f19601f8301168501019250505092915050565b80356001600160a01b03811681146105b5575f80fd5b919050565b5f80604083850312156105cb575f80fd5b6105d48361059f565b946020939093013593505050565b5f805f606084860312156105f4575f80fd5b6105fd8461059f565b925061060b6020850161059f565b9150604084013590509250925092565b5f6020828403121561062b575f80fd5b6106348261059f565b9392505050565b5f806040838503121561064c575f80fd5b6106558361059f565b91506106636020840161059f565b90509250929050565b600181811c9082168061068057607f821691505b60208210810361069e57634e487b7160e01b5f52602260045260245ffd5b50919050565b8082018082111561022757634e487b7160e01b5f52601160045260245ffdfea2646970667358221220ec63e73aaa07c9402c8894dda129081016904cf6fe47c7f3c13f2fb6f1a1059b64736f6c6343000819003300000000000000000000000000000000000000000000021e19e0c9bab2400000");
  TEST_CASE("EVM ERC20 Tests", "[contract][evm]") {
    SECTION("ERC20 Creation") {
      SDKTestSuite sdk = SDKTestSuite::createNewEnvironment("testERC20CreationEVM");
      // const TestAccount& from, const Address& to, const uint256_t& value, Bytes data = Bytes()
      auto erc20Address = sdk.deployBytecode(erc20bytecode);
      // Now for the funny part, we are NOT a C++ contract, but we can
      // definitely take advantage of the templated ABI to interact with it
      // as the encoding is the same.
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::name) == "TestToken");
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::symbol) == "TST");
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::decimals) == 18);
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::totalSupply) == uint256_t("10000000000000000000000"));
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("10000000000000000000000"));
    }

    SECTION("ERC20 transfer") {
      SDKTestSuite sdk = SDKTestSuite::createNewEnvironment("testERC20CreationEVM");
      // const TestAccount& from, const Address& to, const uint256_t& value, Bytes data = Bytes()
      auto erc20Address = sdk.deployBytecode(erc20bytecode);
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::name) == "TestToken");
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::symbol) == "TST");
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::decimals) == 18);
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::totalSupply) == uint256_t("10000000000000000000000"));
      REQUIRE(sdk.callViewFunction(erc20Address, &ERC20::balanceOf, sdk.getChainOwnerAccount().address) == uint256_t("10000000000000000000000"));
      Address owner = sdk.getChainOwnerAccount().address;
      Address to(Utils::randBytes(20));

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
    }
  }
}