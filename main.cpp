#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

//card structure
struct Card{
  string suit;
  string rank;
  int value;
};

// Function to generate a deck of cards
vector<Card> GenerateDeck() {
    vector<string> suits = { "Hearts", "Diamonds", "Clubs", "Spades" };
    vector<string> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

    vector<Card> deck;

    for (const auto& suit : suits) {
        for (const auto& rank : ranks) {
            int value = (rank == "A") ? 11 : ((rank == "J" || rank == "Q" || rank == "K") ? 10 : stoi(rank));
            deck.push_back({ suit, rank, value });
        }
    }
      return deck;
}

// Function to shuffle the deck
void ShuffleDeck(vector<Card>& deck) {
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < deck.size(); ++i) {
        int j = rand() % deck.size();
        swap(deck[i], deck[j]);
    }
}
// Function to calculate the total value of a hand
int GetHandValue(const vector<Card>& hand) {
    int totalValue = 0;
    int numAces = 0;

    for (const auto& card : hand) {
        totalValue += card.value;
        if (card.rank == "A") {
            numAces++;
        }
    }

    while (totalValue > 21 && numAces > 0) {
        totalValue -= 10;
        numAces--;
    }

    return totalValue;
}


// Function to display a player's hand
void DisplayHand(const vector<Card>& hand) {
    for (const auto& card : hand) {
        cout << card.rank << " of " << card.suit << endl;
    }
    cout << "Total value: " << GetHandValue(hand) << endl;
}

// Function to deal a card to a player
void DealCard(vector<Card>& hand, vector<Card>& deck) {
    hand.push_back(deck.back());
    deck.pop_back();
}

// Function to check if a player has blackjack
bool HasBlackjack(const vector<Card>& hand) {
    return (hand.size() == 2 && GetHandValue(hand) == 21);
}

//calculate advantage
string calcAdvantage(vector<Card>& playerHand, vector<Card>& dealerHand)
  {
    string probToString = "\nYou have a ";
    int min = GetHandValue(playerHand)- dealerHand[0].value;
    int max = 21 - dealerHand[0].value;
    int amountPlayed = 0;
    double probDealerWin = 0.0;
    double probPlayerWin = 100.0;
    double advantage = 0.0;
    for(int i; i < playerHand.size(); i++)
      {
        if((playerHand[i].value <= max) && (playerHand[i].value >= min) )
        {
          amountPlayed++;
        }
      }
    if((dealerHand[0].value <= max) && (dealerHand[0].value >= min) )
        {
          amountPlayed++;
        }
    probDealerWin = ((((max-min)*4)-amountPlayed)/(52.0-(playerHand.size() + 1))) *100;
    probPlayerWin -= probDealerWin;
    probToString += to_string(probPlayerWin) + " % chance of winning.";
    advantage = probPlayerWin - probDealerWin;
    probToString += "\nYour advantage is " + to_string(advantage) +"%";
    return probToString;
  }
//calculate probability of certain card
double probOf(vector<Card>& playerHand, vector<Card>& dealerHand, int value)
{
  double prob;
  int amountPlayed = 0;
  for (int i = 0; i < playerHand.size(); i++)
  {
    if (playerHand[i].value == value)
    {
      amountPlayed++;
    }
  }
  if(dealerHand[0].value ==  value)
  {
    amountPlayed++;
  }
  if (value == 10)
  {
    prob = ((16 - amountPlayed) / (52.0 - (playerHand.size() + dealerHand.size()))) * 100.0;
  }
  else
  {
    prob = ((4 - amountPlayed) / (52.0 - (playerHand.size() + dealerHand.size()))) * 100.0;
  }


  return prob;
}

// probability of needed card toString
string probToString(vector<Card>& playerHand, vector<Card>& dealerHand)
{
  
    double prob = 0.0;
    int valueNeeded = 21 - GetHandValue(playerHand);
    string probToString = "The probability of getting a card with value of " +   to_string(valueNeeded) + " is ";
    if(valueNeeded>11)
    {
      return "There is no value to give 21.";
    }
  else 
    {
      prob = probOf(playerHand, dealerHand, valueNeeded);
    }

    return probToString + to_string(prob) +"%";
}


// Function to play the player's turn
void PlayPlayerTurn(vector<Card>& playerHand, vector<Card>& dealerHand, vector<Card>& deck) {
    string choice;
    while (choice != "stand") {
        cout << "\n"<<probToString(playerHand, dealerHand);
        cout << calcAdvantage(playerHand, dealerHand) <<endl;
        cout << "Player's turn. Enter 'hit' or 'stand': ";
        cin >> choice;

        if (choice == "hit") {
            DealCard(playerHand, deck);
            DisplayHand(playerHand);

            if (GetHandValue(playerHand) > 21) {
                cout << "Bust! You lose." << endl;
                return;
            }
        }
    }
}

// Function to play the dealer's turn
void PlayDealerTurn(vector<Card>& dealerHand, vector<Card>& deck, const vector<Card>& playerHand) {
    
    if(GetHandValue(playerHand)> 21)
    {
      return;
    }
  else
    { 
      cout << "Dealer's turn." << endl;
      while (GetHandValue(dealerHand) < 17) {
        DealCard(dealerHand, deck);

        if (GetHandValue(dealerHand) > 21) {
          cout << "Dealers hand: ";
            DisplayHand(dealerHand);
            cout << "Dealer busts! You win." << endl;
            return;
        }
    }

    if (GetHandValue(dealerHand) > GetHandValue(playerHand)) {
      cout << "Dealers hand: ";
      DisplayHand(dealerHand);
      cout << "Dealer wins." << endl;
    } else if (GetHandValue(dealerHand) < GetHandValue(playerHand)){
      cout << "Dealers hand: ";
        DisplayHand(dealerHand);
        cout << "You win!" << endl;
    } else {
      cout << "Dealers hand: ";
        DisplayHand(dealerHand);
        cout << "It's a tie." << endl;
    }}
   
}

int main() {
  bool playAgain = true;
  string play = "";
  while(playAgain)
    {
    vector<Card> deck = GenerateDeck();
    ShuffleDeck(deck);

    vector<Card> playerHand;
    vector<Card> dealerHand;

    // Deal initial cards
    DealCard(playerHand, deck);
    DealCard(dealerHand, deck);
    DealCard(playerHand, deck);
    DealCard(dealerHand, deck);

    cout << "Player's hand:" << endl;
    DisplayHand(playerHand);


    cout << "\nDealer's hand:" << endl;
    cout << dealerHand[0].rank << " of " << dealerHand[0].suit << endl;

    if (HasBlackjack(playerHand)) {
        if (HasBlackjack(dealerHand)) {
            cout << "Push. It's a tie." << endl;
        } else {
            cout << "Blackjack! You win!" << endl;
        }
    } else {
        PlayPlayerTurn(playerHand,dealerHand, deck);
        PlayDealerTurn(dealerHand, deck, playerHand);
    }
      cout << "Play again? y/n\n";
      cin >> play;
      if(play == "n")
      {
        playAgain = false;
      }
    }

    return 0;
}