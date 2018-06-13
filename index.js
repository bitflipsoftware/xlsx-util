const nativeModule = require('bindings')('xlsx')

const readFile = (filepathString, hasHeadersBool, headerTransformFunc) => new Promise( (resolve, reject) => {

    try {
        if (!filepathString || typeof filepathString !== 'string') {
            throw new Error('first argument must be a filepath in the form of a string')
        }

        let transform = (arr) => arr

        if(!headerTransformFunc) {
            transform = headerTransformFunc
        } else if ({}.toString.call(headerTransformFunc) !== '[object Function]') {
            throw new Error('third argument must be a function')
        }

        nativeModule.readXlsxAsync(filepathString, hasHeadersBool, transform, (result, error) => {
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
