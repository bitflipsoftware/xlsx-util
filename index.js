const nativeModule = require('bindings')('xlsx')

const privatePromiseWrap = (filepathString, doReadHeaders, headerTransformFunc)  => new Promise( (resolve, reject) => {
    try {
        if (!filepathString || typeof filepathString !== 'string') {
            throw new Error('first argument must be a filepath in the form of a string')
        }

        nativeModule.readXlsxAsync(filepathString, doReadHeaders, headerTransformFunc, (error, result) => {
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

const readFile = (filepathString) => new Promise( async (resolve, reject) => {
    try {
        const result = await privatePromiseWrap(filepathString, false, null)
        resolve(result)
    } catch (e) {
        reject(e)
    }
})

const readFileWithHeaders = (filepathString) => new Promise( async (resolve, reject) => {
    try {
        const result = await privatePromiseWrap(filepathString, true, null)
        resolve(result)
    } catch (e) {
        reject(e)
    }
})

const readFileWithHeaderTransform = (filepathString, headerTransformFunc) => new Promise( async (resolve, reject) => {
    try {
        const result = await privatePromiseWrap(filepathString, true, headerTransformFunc)
        resolve(result)
    } catch (e) {
        reject(e)
    }
})

module.exports = {
    readFile,
    readFileWithHeaders,
    readFileWithHeaderTransform,
}
