#include "zdice/core/dice/dice_bag.hpp"
#include "zdice/core/dice/dice.hpp"

void DiceBag::push(Dice dice) { m_dices.push_back(dice); }

void DiceBag::pick(size_type n, vec_dice& destination)
{
  //<! Faz um embaralhamento aleatório dos dados
  shuffle(m_dices);

  for (size_type i{ 0 }; i < n and not m_dices.empty(); ++i)
  {
    destination.push_back(m_dices.back());
    m_dices.pop_back();
  }

  // destination.insert(destination.end(), m_dices.begin(), m_dices.begin() + n);

  // // [!] Remove os dados escolhidos do saco
  // m_dices.erase(m_dices.begin(), m_dices.begin() + n);
}

void DiceBag::place(vec_dice& source)
{
  while (not source.empty())
  {
    m_dices.push_back(source.back());
    source.pop_back();
  }
  // //<! Adiciona os dados de volta ao saco
  // m_dices.insert(m_dices.end(), dices.begin(), dices.end());
  // return m_dices; //<! Retorna o saco atualizado
}
