import sys
sys.setrecursionlimit(10**7)

class Trie:

  class NodeNotFound(Exception):
    pass

  def __init__(self, letters):
    self.children = {}
    self.isWordEnd = False
    self.addString(letters)

  def addString(self, letters):
    if not letters:
      self.isWordEnd = True
      return self
    letter = letters[0]
    if letter in self.children:``
      self.children[letter].addString(letters[1:])
    else:
      self.children[letter] = Trie(letters[1:])

  def display(self):
    for key, value in self.children.items():
      print(str(value.display()) + key)

  def childNodes(self, root):
    if not root:
      return self.children.keys()
    elif root[0] not in self.children:
      return []
    else:
      return list(self.children[root[0]].childNodes(root[1:]))

  def checkWord(self, letters):
    if not letters:
      return self.isWordEnd
    else:
      if letters[0] in self.children:
        return self.children[letters[0]].checkWord(letters[1:])
      else:
        return False

  def node(self, start):
    if start == "":
      return self
    if start[0] in self.children:
      return self.children[start[0]].node(start[1:])
    else:
      raise self.NodeNotFound

  #returns a pair every time
  def legalPrefixes(self, start, words, path):
    if self.isWordEnd and path and start:
      words.append(path)
    if start == "":
      return (self, words)
    if start[0] in self.children:
      return self.children[start[0]].legalPrefixes(start[1:], words,
                                                   path + start[0])
    else:
      raise self.NodeNotFound

  def wordsStarting(self, start):
    try:
      node = self.node(start)
      return node.collectWordList(start, "", [])
    except self.NodeNotFound:
      return []

  # given a string start, returns a list of strings [n] such that n and start agree on all indices up to the length of the shorter of the two
  def overlappingWords(self, start):
    try:
      legalPrefixesAndStart = self.legalPrefixes(start, [], "")
      root = legalPrefixesAndStart[0]
      return root.collectWordList(start, "", legalPrefixesAndStart[1])
    except self.NodeNotFound:
      return []

  def collectWordList(self, start, path, list):
    if self.isWordEnd:
      list.append(start + path)
    for letter, child in self.children.items():
      child.collectWordList(start, path + letter, list)
    return list

  def toWordList(self):
    wordSet = set()
    self.collectWordList("", wordSet)
    return wordSet


dictionary = Trie("")
yranoitcid = Trie("")
palindromes = []

# words = ["able", "was", "i", "ere", "saw", "elba"]
# words = ["dammit", "im", "mad"]
# words = ["evil", "evangelion", "nonlive"]
# words = ["abcdefghijkl", "mlkj", "ih", "gfe", "dc", "ba"]
# words = ["ab", "cdefg", "hihg", "fedcba"]
# words = ["do", "geese", "see", "god"]
# for w in words:
#   dictionary.addString(w)
#   yranoitcid.addString(w[::-1])


# arguments: a b
# assumptions about arguments: strings where one contains the other at the start
# if the assumption does not hold, then
# returns: tuple of (string (overlap), string chosen to be longer)
# modifies:


# arguments:
# behavior:
def palindrome(startingPrefix=""):

  def thisSucc(palindrome, continuation):
    pali = cleanUp(palindrome)
    # print(pali)
    with open("output.txt", "a") as f:  # Append each palindrome to the file
      f.write(pali + "\n")

    palindromes.append(pali)
    continuation("ran out")
  

  possibleWords = dictionary.wordsStarting(startingPrefix)
  possibleWords.extend(filter(dictionary.checkWord,
                              prefixesOf(startingPrefix)))
  if not possibleWords:
    print("Error: no words starting with that prefix")
    return
    
  findPalindrome("",
                 "",
                 possibleWords,
                 0,
                #  lambda palindrome, continuation: continuation("ran out")
                #  if input("Palindrome: " + cleanUp(palindrome) + "\nAnother? (y or n)")
                #  == "y" else None,
                 thisSucc,
                 lambda error="failed": print("Error: " + error))



def isPalindrome(a_string):
  return a_string == a_string[::-1]


# arguments:
# - emordnilap:string is the beginning/reversed ending of our ultimate palindrome
# - overhang is the letters which must be the beginning of the next word to be appended to emordnilap
# - depth is the depth of recursion/number of words chosen so far
# - succ is what to do if we have succeeded in finding a palindrome
# - fail is what to do if we reach a failstate on this branch (max length reached, )
# Behavior:
# If a palindrome is reached, succ will be called
# If no palindrome can be reached from the prefix within the depth, fail will be called on an error string
def findPalindrome(emordnilap,
                   overhang,
                   possibleWords,
                   depth,
                   succ,
                   fail,
                   isForwards=True):
  if not possibleWords:
    return fail("No possibilities at depth " + str(depth))
  elif depth > MAX_PALINDROME_LEN:
    return fail("Error: Hit maximum search depth at " + emordnilap)
  elif isPalindrome(overhang) and emordnilap:
    if isForwards: return succ(emordnilap + " " + overhang + emordnilap[::-1] + " " + str(depth), fail)
    else: return succ(emordnilap + overhang + " " + emordnilap[::-1] + " " + str(depth), fail)
  else:
    smallerPrefix, largerPrefix, newDirection = (overhang, possibleWords[0], not isForwards) if len(possibleWords[0]) >= len(overhang) else (possibleWords[0], overhang, isForwards)
    newOverhang = largerPrefix[len(smallerPrefix):]

    if newDirection:
      newPossibilities = dictionary.wordsStarting(newOverhang)
      newPossibilities.extend([w for w in prefixesOf(newOverhang) if dictionary.checkWord(w[::-1])])
    else:
      newPossibilities = yranoitcid.wordsStarting(newOverhang)
      newPossibilities.extend(filter(yranoitcid.checkWord, prefixesOf(newOverhang)))

    findPalindrome(
      emordnilap + "1" + smallerPrefix if isForwards else emordnilap + "2" + smallerPrefix,
      newOverhang, 
      newPossibilities,
      depth + 1,
      succ,
      (lambda _: findPalindrome(emordnilap, 
                                overhang, 
                                possibleWords[1:], 
                                depth, 
                                succ, 
                                fail,
                                isForwards)),
      newDirection)


def prefixesOf(word):
  return [word[:n] for n in range(1, len(word))]

def cleanUp(str):
  newStr = ""
  for pos in range(len(str)):
    if pos < len(str) / 2:
      if str[pos] == '1': 
        newStr += " "
      elif str[pos] == '2':
        newStr += ""
      else: 
        newStr += str[pos]
    else: 
      if str[pos] == '1': 
        newStr += ""
      elif str[pos] == '2':
        newStr += " "
      else: 
        newStr += str[pos]
  return newStr

    
num = int(input("min length: "))

MAX_PALINDROME_LEN = 6

MIN_WORD_LEN = num

for line in open("cel.txt", "r"):
  if (len(line.strip())) >= MIN_WORD_LEN:
    dictionary.addString(line.strip().lower())
    yranoitcid.addString(line.strip().lower()[::-1])
dictionary.addString("a")
yranoitcid.addString("a")

print("done")

attempt = input("start: ")
dictionary.addString(attempt.lower())
yranoitcid.addString(attempt.lower()[::-1])
palindrome(attempt)
print(palindromes)
#  with open("output.txt", "a") as f:  # Append each palindrome to the file
#       f.write(palindromes)