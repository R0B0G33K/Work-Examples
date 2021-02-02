'use strict';

module.exports = {
  up: async (queryInterface, Sequelize) => {
    return queryInterface.createTable(
      'admins',{
          email: {
              type: Sequelize.STRING,
              primaryKey: true
          }
      })
  },

  down: async (queryInterface, Sequelize) => {
    await queryInterface.dropTable('admins');
  }
};
