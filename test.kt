
interface MyStack {
    fun add(data: Any): Boolean

    fun pop(): Any?

    fun isEmpty(): Boolean

    fun print()
}

class ListMyStack : MyStack {
    private val maxsize = 100
    private var currentDatas: Array<Any?> = arrayOfNulls(maxsize)
    private var pointer = -1

    override fun add(data: Any): Boolean {
        if (pointer < maxsize - 1) {
            pointer++
            currentDatas[pointer] = data
            return true
        } else {
            return false
        }
    }

    override fun pop(): Any? {
        if (isEmpty()) {
            return null
        } else {
            val deletedElem = currentDatas[pointer]
            pointer--
            return deletedElem
        }
    }

    override fun isEmpty(): Boolean {
        return pointer == -1
    }

    override fun print() {
        currentDatas.indices.take(pointer + 1).forEach { println(currentDatas[it]) }
    }
}

fun main() {
    val a = ListMyStack()
// add 20
    for (i in 1..20) {
        a.add("Element $i")
    }
//
    println("After adding 20 elements:")
    a.print()
// pop 20
    println("\nRemoving elements:")
    repeat(21) {
        val deletedElement =
            a.pop()
        println("Deleted: $deletedElement")
    }
//
    println("\nAfter removing 20 elements:")
    a.print()
}
