// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
//#include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(HiddenWords,*WordListPath,ValidWord);
    SetupGame(); 
    //PrintLine(FString::Printf(TEXT("Teh HiddenWord is: %s"), *HiddenWord));//Debug Line * is the address of the hidden word string
}

bool UBullCowCartridge::ValidWord(const FString& RandomWord)
{
        if(RandomWord.Len() < 9 &&  RandomWord.Len() > 3 && IsIsogram(RandomWord)){
            return true;
        }
    return false;
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
/* if game is over then do ClearScreen and SetupGame() the game
    else Checking Player Guess*/
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    } else {
        ClearScreen();
        PrintLine(TEXT("Hidden Word: %s"),*HiddenWord);
        ProcessGuess(PlayerInput);
    }
}
void UBullCowCartridge::SetupGame()
{
    //Set the Hidden Word and reset BullCow count
    HiddenWord = HiddenWords[FMath::RandRange(0,HiddenWords.Num()-1)];

    Lives = HiddenWord.Len();//Set lives
    bGameOver = false;
        //Welcoming the Player
    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Hidden Word: %s"),*HiddenWord);
    //PrintLine(TEXT("There are %i Valid Words."),HiddenWords.Num());
    PrintLine(TEXT("Guess the %i letter word!"),HiddenWord.Len());//Magic Number remove
    PrintLine(TEXT("Type in your guess."));
    PrintLine(TEXT("Press enter to continue..."));
    PrintLine(TEXT("You have %i lives."),Lives);


}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for(int32 i=0;i<Word.Len();i++){
        for(int32 j=i+1;j<Word.Len();j++){
            if(Word[i]==Word[j]){
                return false;
            }
        }
    }
    return true;
}

void UBullCowCartridge::BullCow(const FString& Word,int32 NumberOfBulls,int32 NumberOfCows)
{
    FString LongestString;
    FString ShortestString;
    if(HiddenWord.Len()!=Word.Len())
    {
        LongestString =(HiddenWord.Len()>Word.Len()) ? HiddenWord : Word;
        ShortestString =(HiddenWord.Len()<Word.Len()) ? HiddenWord : Word;
    } else {
        LongestString = HiddenWord;
        ShortestString = Word;
    }
    for(int32 i=0;i < ShortestString.Len();i++)
    {
        if(ShortestString[i]==LongestString[i])
        {
            NumberOfBulls++;
            continue;
        }

        for(int32 j = 0; j<LongestString.Len(); j++)
        {
            if(ShortestString[i]==LongestString[j] && j!=i)
            {
                NumberOfCows++;
                break;
            }       
        }
    }
    PrintLine(TEXT("BULLS: %i, COWS: %i"),NumberOfBulls,NumberOfCows);
}


void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    FBullCowCount Count;
    if(Guess == HiddenWord)
    {
        ClearScreen();
        PrintLine(TEXT("You win!"));
        PrintLine(TEXT("The Hidden word was: %s"),*Guess);
        EndGame();
        return;
    } 
    //Prompt to Guess Again
    //check if right number of Characters
    if(Guess.Len() < HiddenWord.Len())
    {
        ClearScreen();
        PrintLine(TEXT("Hidden Word: %i Characters long."),HiddenWord.Len());
        return;
    }
    //Check if Isogram
    if(!IsIsogram(Guess)){
        ClearScreen();
        PrintLine("Isograms Dont repeat letters. \nTry again");
        return;
    }
    PrintLine(TEXT("Life lost, %i left."), --Lives);//lose life
    BullCow(Guess,Count.Bulls,Count.Cows);
    if(Lives == 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left."));
        PrintLine(TEXT("You guessed %s"), *Guess);
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }
    PrintLine(TEXT("You Guessed: %s"),*Guess);
}