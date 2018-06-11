const xlsx = require('../index')
const path = require('path')

describe('getSheetNames', () => {
    it('should return data', async (done) => {
        const filepath = path.join(__dirname, 'get-data.xlsx')
        const result = await xlsx.getData(filepath);

        expect(result).toBeTruthy();
        done();
    })
})
