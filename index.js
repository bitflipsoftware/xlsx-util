const nativeModule = require('bindings')('xlsx')

const readFile = (filepathString) => new Promise( (resolve, reject) => {

    try {
        if (!filepathString || typeof filepathString !== 'string') {
            throw new Error('first argument must be a filepath in the form of a string')
        }

        nativeModule.readXlsxAsync(filepathString, (error, result) => {
            if(error) {
                reject(error)
            } else if (result) {
                resolve(result)
            } else {
                reject(new Error('unknown error occurred and the result is null'))
            }
        })

    } catch (e) {
        reject(e)
    }
})

module.exports = {
    readFile
}
