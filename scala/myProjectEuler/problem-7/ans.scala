object ans
{
  def sieve(inputList : List[Int]):List[Int] = {
    var Result = List(2)
    var searchList = inputList

    do{
      searchList = searchList.filter(_ % Result.last != 0)
      Result = Result:+ searchList.head
      searchList = searchList.tail
    }while((Result.last*Result.last) < (searchList.last).toDouble)

    Result = Result ::: searchList
    
    return Result
  }
  def main(args : Array[String])
  {
    var searchList = Range(3, 100001).toList
    
    println(sieve(searchList))
  }
}
