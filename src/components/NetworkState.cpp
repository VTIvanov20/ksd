// #include "./NetworkController.hpp"

// void NetworkState::SetCode(std::string strCode)
// {
//     codeSize = strCode.size();
//     code = new char[codeSize];

//     std::copy(code, code + codeSize, strCode);
// }

// void SetCardValue(std::shared_ptr<Node<CardType>> card, AtomicNode<CardType> *atomicCard)
// {
//     if (card == nullptr) return;

//     atomicCard->val = card->val;

//     SetCardValue(card->next, atomicCard->next);
// }

// void ClearCardValues(AtomicNode<CardType> *atomicNode)
// {
//     if (atomicNode == nullptr) return;

//     ClearCardValues(atomicNode->next);

//     delete atomicNode;
// }

// void NetworkState::SetCardStates(std::vector<BeginningNode<CardType>> vecCardState)
// {
//     if (cardState != nullptr)
//     {
//         for (int i = 0; i < cardStateSize; i++)
//         {
//             ClearCardValues(cardState[i].bottomNext);
//             ClearCardValues(cardState[i].topNext);
//         }

//         delete[] cardState;
//     }

//     cardState = new AtomicBeginningNode<CardType>[vecCardState.size()];
//     cardStateSize = vecCardState.size();

//     for (int i = 0; i < vecCardState.size(); i++)
//     {
//         SetCardValue(vecCardState[i].bottomNext, cardState[i].bottomNext);
//         SetCardValue(vecCardState[i].topNext, cardState[i].topNext);
//     }
// }

// void NetworkState::SetPlayerDeck(std::vector<CardType> vecPlayerDeck)
// {
//     if (playerDeck != nullptr)
//         delete[] playerDeck;
    
//     playerDeck = new CardType[vecPlayerDeck.size()];
//     playerDeckSize = vecPlayerDeck.size();

//     std::copy(playerDeck, playerDeck + playerDeckSize, vecPlayerDeck.begin());
// }

// std::string NetworkState::GetCode()
// {
//     return std::string { code };
// }

// std::vector<BeginningNode<CardType>> GetCardStates()
// {
//     return std::vector<BeginningNode<CardType>>();
// }

// std::vector<CardType> GetPlayerDeck()
// {
//     return std::vector<CardType>();
// }