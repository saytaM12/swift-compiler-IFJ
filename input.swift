let a = 5+5*8
let b = 8+(5+5)*"ahoj"
// a = 5+5*8
// // Program 1: Vypocet faktorialu (iterativne)
// /* Hlavni telo programu */
// func hello(from x: Int, to y: Int) {
//     write("Zadejte cislo pro vypocet faktorialu\n")
// }
// let b : Int = 6.5
// let a : Int? = readInt()
// if let a {
//     if let b {write("Faktorial nelze spocitat\n")
//     } else {
//         var a = Int2Double(a)
//             var vysl : Double = 1
//             while (a > 0) {
//                 vysl = vysl * a
//                     a = a - 1
//             }
//         write("Vysledek je: ", vysl, "\n")
//     }
// } else {
//     write("Chyba pri nacitani celeho cisla!\n")
// }
//     // Program 2: Vypocet faktorialu (rekurzivne)
//     // Hlavni telo programu
//     write("Zadejte cislo pro vypocet faktorialu: ")
// let inp = readInt()
//     // pomocna funkce pro dekrementaci celeho cisla o zadane cislo
//     func decrement(of n: Int, by m: Int) -> Int {
//         return n - m
//     }
// // Definice funkce pro vypocet hodnoty faktorialu
// func factorial(_ n : Int) -> Int {
//     var result : Int?
//         if (n < 2) {
//             result = 1
//         } else {
//             let decremented_n = decrement(of: n, by: 1)
//                 let temp_result = factorial(decremented_n)
//                 result = n * temp_result
//         }
//     return result!
// }
// // pokracovani hlavniho tela programu
// if let inp {
//     if (inp < 0) { // Pokracovani hlavniho tela programu
//         write("Faktorial nelze spocitat!")
//     } else {
//         let vysl = factorial(inp)
//             12
//             write("Vysledek je: ", vysl)
//     }
// } else {
//     write("Chyba pri nacitani celeho cisla!")
// }
// /* Program 3: Prace s retezci a vestavenymi funkcemi */
// var str1 = "Toto je nejaky text v programu jazyka IFJ23"
// let str2 = str1 + ", ktery jeste trochu obohatime"
//     write(str1, "\n", str2, "\n")
// let i = length(str1)
//     write("Pozice retezce \"text\" v str2: ", i, "\n")
//     write("Zadejte serazenou posloupnost vsech malych pismen a-h, ")
// let newInput = readString()
//     if let newInput {
//         str1 = newInput
//             while (str1 != "abcdefgh") {
//                 write("Spatne zadana posloupnost, zkuste znovu:\n")
//                     let tmp = readString()
//                     str1 = tmp ?? ""
//             }
//     } else {}
