const nativeModule = require('bindings')('xlsx')

const privatePromiseWrap = (filepathString, doReadHeaders, headerTransformObj, columnsToDeleteArr, doPascalCase, pascalWords, stringColumns)  => new Promise( (resolve, reject) => {
    try {
        if (!filepathString || typeof filepathString !== 'string') {
            throw new Error('first argument must be a filepath in the form of a string')
        }

        nativeModule.readXlsxAsync(filepathString, doReadHeaders, headerTransformObj, columnsToDeleteArr, doPascalCase, pascalWords, stringColumns, (error, result) => {
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
        const columnsToDeleteArr = []
        const result = await privatePromiseWrap(filepathString, false, null, columnsToDeleteArr, false, null, [])
        resolve(result)
    } catch (e) {
        reject(e)
    }
})

const readFileWithHeaders = (filepathString) => new Promise( async (resolve, reject) => {
    try {
        const columnsToDeleteArr = []
        const result = await privatePromiseWrap(filepathString, true, null, columnsToDeleteArr, false, null, [])
        resolve(result)
    } catch (e) {
        reject(e)
    }
})

const readFileWithHeaderTransform = (filepathString, headerTransformObj) => new Promise( async (resolve, reject) => {
    try {
        const columnsToDeleteArr = []
        const result = await privatePromiseWrap(filepathString, true, headerTransformObj, columnsToDeleteArr, false, null, [])
        resolve(result)
    } catch (e) {
        reject(e)
    }
})

const readFileWithHeaderTransformAndDelete = (filepathString, headerTransformFunc, columnsToDeleteArr) => new Promise( async (resolve, reject) => {
    try {
        const result = await privatePromiseWrap(filepathString, true, headerTransformFunc, columnsToDeleteArr, false, null, [])
        resolve(result)
    } catch (e) {
        reject(e)
    }
})

const readFileWithHeaderTransformDeleteAndPascalCase = (filepathString, headerTransformFunc, columnsToDeleteArr, pascalWords) => new Promise( async (resolve, reject) => {
    try {
        const result = await privatePromiseWrap(filepathString, true, headerTransformFunc, columnsToDeleteArr, true, pascalWords, [])
        resolve(result)
    } catch (e) {
        reject(e)
    }
})

const readFileWithHeaderTransformDeleteAndPascalCaseAndStringColumns = (filepathString, headerTransformFunc, columnsToDeleteArr, pascalWords, stringColumns) => new Promise( async (resolve, reject) => {
    try {
        const result = await privatePromiseWrap(filepathString, true, headerTransformFunc, columnsToDeleteArr, true, pascalWords, stringColumns)
        resolve(result)
    } catch (e) {
        reject(e)
    }
})

const replaceAll = (theString, findStr, replaceStr) => {

    if(theString === null || typeof theString !== 'string')
    {
        throw new Error('theString must be a string')
    }

    if(findStr === null || typeof findStr !== 'string')
    {
        throw new Error('findStr must be a string')
    }

    if(replaceStr === null || typeof replaceStr !== 'string')
    {
        throw new Error('replaceStr must be a string')
    }

    return nativeModule.replaceAll(theString, findStr,replaceStr)
}


const toLower = (inputStr) => {

    if(inputStr === null || typeof inputStr !== 'string')
    {
        throw new Error('inputStr must be a string')
    }

    return nativeModule.toLower(inputStr)
}

module.exports = {
    readFile,
    readFileWithHeaders,
    readFileWithHeaderTransform,
    readFileWithHeaderTransformAndDelete,
    readFileWithHeaderTransformDeleteAndPascalCase,
    readFileWithHeaderTransformDeleteAndPascalCaseAndStringColumns,
    replaceAll,
    toLower,
}
