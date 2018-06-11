const xlsx = require('../index')
const path = require('path')

describe('getSheetNames', () => {
    it('should return data', async (done) => {
        const filepath = path.join(__dirname, 'get-data.xlsx')
        const isFirstRowHeaders = true;
        
        const options = {
            headerReplacements: {
                hello: "goodbye"
            }
        }

        const result = await xlsx.getData(filepath, isFirstRowHeaders, options)
        expect(result).toBeTruthy();
        done();
    })
})
